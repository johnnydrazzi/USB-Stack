/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 04/03/2019
 * 
 * MSD Internal Example (for PIC16F145X).
 * Copyright (C) 2017-2019  John Izzard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <xc.h>
#include <stdint.h>
#include "fuses.h"
#include "config.h"
#include "../Shared_Files/USB.h"
#include "../Shared_Files/usb_msd.h"
#include "../Shared_Files/flash.h"

#ifndef MSD_LIMITED_RAM
#error "Must use MSD_LIMITED_RAM option for this example."
#endif

/* PIC16F145X ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            | BOOT LOADER  |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x2000 (8KB) *Only 4KB usable due to 14-bit size words
    0x03FFF |______________|
    0x10000 |              |
            | CONFIG WORDS | 0x0E
    0x1000D |______________|
 */

#if _HTC_EDITION_ == 0 // Free Version - put ROM range as 0-27FF
#error "Don't even bother trying to do this in Free Mode!"
#else                  // Standard/Pro Version - put ROM range as 0-1FFF
#define FLASH_SPACE_START     0x01000
#endif
#define END_OF_FLASH          0x02000


/*
 * The code directly below will pre-format the drive in such a way that we make use of most of the space.
 * Using our format method, we get 2.5KB volume.
 * 
 * You can gain another 1KB when you disable System Volume Information folder creation on 
 * removable drives in windows (Windows 8.1/10).
 * 
 * 1. Type gpedit.msc in the Run dialog box (Windows + R) to open Local Group Policy Editor.
 * 2. Computer Configuration -> Administrative Templates -> Windows Components -> Search
 * 3. Find "Do not allow locations on removable drives to be added to libraries" and double click on it.
 * 4. Change setting to Enable, click Apply and then OK.
 * 
 * Not sure of a solution to MAC or if Linux does this too...
 */
#define BOOT_START            (FLASH_SPACE_START)
#define SIGNATURE_WORD_START  0x11FE
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     0x1200
#define FAT_DEFAULT_SIZE      4
#define FAT_REST_OF_START     0x1204
#define ROOT_ENTRY_START      0x1400

/** Boot Sector */
const struct{
    uint8_t jmpBoot[3];
    uint8_t OEMName[8];
    uint16_t BytesPerSec;
    uint8_t SecPerClus;
    uint16_t RsvdSecCnt;
    uint8_t NumFATs;
    uint16_t RootEntCnt;
    uint16_t TotSec16;
    uint8_t Media;
    uint16_t FATSz16;
    uint16_t SecPerTrk;
    uint16_t NumHeads;
    uint32_t HiddSec;
    uint32_t TotSec32;
    uint8_t DrvNum;
    uint8_t Reserved1;
    uint8_t BootSig;
    uint8_t VolID[4];
    uint8_t VolLab[11];
    uint8_t FilSysType[8];
}BOOT16 @BOOT_START = {
    {0xEB,0x3C,0x90},
    {'M','S','D','O','S','5','.','0'},
    BYTES_PER_BLOCK_LE,
    1,
    1,
    1,
    16,
    VOL_CAPACITY_IN_BLOCKS,
    0xF8,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0x29,
    {0x86,0xE8,0xA3,0x56},
    {'U','S','B',' ','D','R','I','V','E',' ',' '},
    {'F','A','T','1','2',' ',' ',' '}
};
const uint8_t SignatureWord[SIGNATURE_WORD_SIZE] @SIGNATURE_WORD_START = {0x55, 0xAA};
const uint8_t Fat12Default[FAT_DEFAULT_SIZE] @FAT_DEFAULT_START = {0xF8,0xFF,0xFF,0x00};

/** Directory Entry Structure */
const struct{
    uint8_t Name[11];
    uint8_t Attr;
    uint8_t NTRes;
    uint8_t CrtTimeTenth;
    uint16_t CrtTime;
    uint16_t CrtDate;
    uint16_t LstAccDate;
    uint16_t FstClusHI;
    uint16_t WrtTime;
    uint16_t WrtDate;
    uint16_t FstClusLO;
    uint32_t FileSize;
}DIR_ENTRY @ROOT_ENTRY_START = {
    {'U','S','B',' ','D','R','I','V','E',' ',' '},
    0x08,
    0,
    0,
    0,
    0,
    0,
    0,
    0x819A,
    0x4E64,
    0,
    0
};

void flash_LED(void);
uint32_t LBA2FlashAddr(uint32_t LBA);
void interrupt ISR(void);

void main(void){
#if (XTAL_USED == NO_XTAL)
    OSCCONbits.IRCF = 0xF;
#endif
#if (XTAL_USED != MHz_12)
    OSCCONbits.SPLLMULT = 1;
#endif
    OSCCONbits.SPLLEN = 1;
    PLL_STARTUP_DELAY();
#if (XTAL_USED == NO_XTAL)
    ACTCONbits.ACTSRC = 1;
    ACTCONbits.ACTEN = 1;
#endif
    LED = 0;
    LED_TRIS = 0;
    flash_LED();
    USB_Init();
    INTCONbits.PEIE = 1;
    USB_Interrupt_Flag = 0;
    USB_Interrupt_Enable = 1;
    INTCONbits.GIE = 1;
    while(USB_STATE != Configured){}
    while(1){
        //USB_Tasks();
        MSD_Tasks();
    }
}

void interrupt ISR(void){
    if(USB_Interrupt_Enable && USB_Interrupt_Flag){
        USB_Tasks();
        USB_Interrupt_Flag = 0;
    }
}

void flash_LED(void){
    uint8_t i;
    for(i=0;i<3;i++){
        LED = 1;
        __delay_ms(500);
        LED = 0;
        __delay_ms(500);
    }
}

void MSD_RxSector(void){
    uint32_t addr;
    uint8_t buffer[64];
    uint8_t i,x;
    addr = LBA2FlashAddr(R_W_10_Vars.LBA); // Convert from LBA address space to flash address space. 
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    if(addr < END_OF_FLASH){ // If address is in flash space.
        if(MSD_EP_IN_LAST_PPB == ODD) Flash_ReadBytes((uint24_t)(addr + ByteOfSect),64,MSD_EP_IN_ODD); // Read flash into MSD_SectData buffer.
        else Flash_ReadBytes((uint24_t)(addr + ByteOfSect),64,MSD_EP_IN_EVEN);
    }
    else{
        if(MSD_EP_IN_LAST_PPB == ODD) USB_RAMSet(0,MSD_EP_IN_ODD,64);
        else USB_RAMSet(0,MSD_EP_IN_EVEN,64);
    }
#else
    if(addr < END_OF_FLASH){
        Flash_ReadBytes((uint24_t)(addr + ByteOfSect),64,buffer);
        for(i=0,x=0;i<64;i+=2,x++) MSD_EP_IN[x] = buffer[i];
        Flash_ReadBytes((uint24_t)(addr + 32 + ByteOfSect),64,buffer);
        for(i=0,x=32;i<64;i+=2,x++) MSD_EP_IN[x] = buffer[i];
    }
    else USB_RAMSet(0,MSD_EP_IN,64);
#endif
}
void MSD_TxSector(void){
    uint32_t addr;
    uint8_t buffer[64];
    uint8_t i,x;
    addr = LBA2FlashAddr(R_W_10_Vars.LBA); // Convert from LBA address space to flash address space.
    if(addr < END_OF_FLASH){ // If address is in flash space.
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        if(MSD_EP_OUT_LAST_PPB == ODD) Flash_EraseWriteBlock((uint24_t)(addr + ByteOfSect),MSD_EP_OUT_ODD);
        else Flash_EraseWriteBlock((uint24_t)(addr + ByteOfSect),MSD_EP_OUT_EVEN);
#else
        for(i=0,x=0;i<64;i+=2,x++){
            buffer[i] = MSD_EP_OUT[x];
            buffer[i+1] = 0xFF;
        }
        Flash_EraseWriteBlock((uint24_t)(addr + ByteOfSect),buffer);
        for(i=0,x=32;i<64;i+=2,x++){
            buffer[i] = MSD_EP_OUT[x];
            buffer[i+1] = 0xFF;
        }
        Flash_EraseWriteBlock((uint24_t)(addr + 32 + ByteOfSect),buffer);
#endif
    }
}

uint32_t LBA2FlashAddr(uint32_t LBA){
    return (LBA*BYTES_PER_BLOCK_LE)+FLASH_SPACE_START;
}