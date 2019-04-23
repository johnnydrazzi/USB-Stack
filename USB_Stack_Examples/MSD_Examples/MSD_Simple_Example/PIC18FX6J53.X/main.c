/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * MSD Simple Example (for PIC18FX6J53).
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

/*
 * USB uC BOOTLOADER INSTRUCTIONS
 * 
 * 1. SETUP PROJECT
 * Right click on your MPLABX project, and select Properties. 
 * Under XC8 global options, click XC8 linker. In the Option categories dropdown, 
 * select Additional options. In the Codeoffset input, you need to put an 
 * offset of 0x2000. (For PIC16F145X offset is in words, therefore 0x1000)
 * 
 * If you are using the a J Series bootloader:
 * In the Option categories dropdown, select Memory Model. In the ROM ranges 
 * input, you need to put a range starting from the Codeoffset (0x2000) to 1KB from last 
 * byte in flash. e.g. For X7J53, 2000-1FBFF is used. This makes sure your code 
 * isn't placed in the same Flash Page as the Config Words. That area is write 
 * protected.
 * 
 * PIC18FX6J53: 2000-FBFF.
 * PIC18FX7J53: 2000-1FBFF.
 * 
 * 2. DOWNLOAD FROM MPLABX
 * You can get MPLABX to download your code every time you press build. 
 * To set this up, right click on your MPLABX project, and select Properties. 
 * Under Conf: [Default], click Building. Check the Execute this line after 
 * build box and place in this line of code (use the drive letter or name of 
 * your device depending on OS):
 * 
 * Windows Example: cp ${ImagePath} E:\ 
 *                  **Needs a space following "\".
 * 
 * OSX Example: cp ${ImagePath} /Volumes/PIC18FX7J53
 * 
 * Linux Example: cp ${ImagePath} /media/PIC18FX7J53
 * 
 * 3. START BOOTLOADER
 * If you have previously loaded a program, reset your device or insert the USB 
 * cable whilst holding down the bootloader button. The bootloader LED will 
 * blink 3 times fast to indicate "bootloader mode" is active. If no program is 
 * present, just insert the USB cable. Your PIC will now appear in My Computer 
 * as a thumb drive.
 * 
 * 4. READ/ERASE
 * If you've previously loaded a program, PROG_MEM.BIN file will exist on the 
 * drive. You can use this file to view the raw binary of your program using a 
 * hex editor. If you wish to erase your program, just delete this file. After 
 * the erase completes, the bootloader will restart and you can load a new program.
 * 
 * 5. EEPROM READ/WRITE/ERASE
 * For PICs that have EEPROM, a EEPROM.BIN file will also exist on the drive. 
 * This file can be used to view your EEPROM and modify it's values. Open the 
 * file in a hex editor, and modify any values and save the file. You can also 
 * erase all the EEPROM values by deleting this file (the bootloader will restart, 
 * and the file will reappear with blank EEPROM).
 * 
 * 6. DOWNLOAD
 * To program, simply drag and drop your hex file or right click your hex file 
 * and select send to PIC18FXXXXX. The bootloader will close and instantly start 
 * running your code. Alternatively, as seen in step two, you can get MPLABX to 
 * download the file automatically after a build.
 * 
 */

/* Emulated FAT12 File System
             ______________
    0x00000 |              |
            |  BOOT SECT   | 0x200 (512B)
    0x001FF |______________|
    0x00200 |              |
            |   FAT SECT   | 0x200 (512B)
    0x003FF |______________|
    0x00400 |              |
            |  ROOT SECT   | 0x200 (512B)
    0x005FF |______________|
    0x00600 |              |
            |  DATA SECT   | 0x1FA00 (126.5KB)
    0x1FFFF |______________|

 */

#define BOOT_SECT_ADDR 0
#define FAT_SECT_ADDR  1
#define ROOT_SECT_ADDR 2
#define DATA_SECT_ADDR 3
#define FILE_SECT_ADDR DATA_SECT_ADDR

#include <xc.h>
#include <stdint.h>
#include "fuses.h"
#include "config.h"
#include "../Shared_Files/USB.h"
#include "../Shared_Files/usb_msd.h"

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
}BOOT16 = {
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
    {'F','A','T','1','6',' ',' ',' '}
};

/** Directory Entry Structure */
typedef struct{
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
}DIR_ENTRY_TYPE;

/** Root Directory Structure */
struct ROOT_DIR_TYPE{
    DIR_ENTRY_TYPE VOL;
    DIR_ENTRY_TYPE FILE1;
};

const uint8_t FILE_DATA[] = "Hello World!";

/** Volume Root Entry */
const struct ROOT_DIR_TYPE ROOT = {
    {
        {'U','S','B',' ','D','R','I','V','E',' ',' '},
        0x08,
        0,
        0,
        0x7BA0,
        0x4B0B,
        0x4B0B,
        0,
        0x7BA0,
        0x4B0B,
        0,
        0
    },
    {
       {'H','E','L','L','O',' ',' ',' ','T','X','T'},
        0x20,
        0,
        0,
        0x7BA0,
        0x4B0B,
        0x4B0B,
        0,
        0x7BA0,
        0x4B0B,
        2,
        sizeof(FILE_DATA)
    }
};

void flash_LED(void);
void interrupt ISR(void);

void main(void){
    OSCTUNEbits.PLLEN = 1;
    PLL_STARTUP_DELAY();
    LED = 0;
    LED_TRIS = 0; // LED tris  
    
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
    
    return;
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
#ifdef MSD_LIMITED_RAM
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    uint8_t *RSC_EPaddress;
    
    if(EPn_Stat.Array[MSD_EP][IN].Last_PPB == ODD) RSC_EPaddress = MSD_EP_IN_ODD;
    else RSC_EPaddress = MSD_EP_IN_EVEN;
#else
#define RSC_EPaddress MSD_EP_IN
#endif
    
    USB_RAMSet(0,RSC_EPaddress,64);
    
    if(R_W_10_Vars.LBA == BOOT_SECT_ADDR){  // If PC is reading the Boot Sector.
        if(ByteOfSect == 0) USB_ROMCopy((const uint8_t*)(&BOOT16),RSC_EPaddress,sizeof(BOOT16));
        else if(ByteOfSect == 448){
            RSC_EPaddress[62] = 0x55;
            RSC_EPaddress[63] = 0xAA;
        }
    }
    else if(R_W_10_Vars.LBA == FAT_SECT_ADDR){ // If PC is reading the FAT.
        if(ByteOfSect == 0){
            RSC_EPaddress[0] = 0xF8;
            RSC_EPaddress[1] = 0xFF;
            RSC_EPaddress[2] = 0xFF;
            RSC_EPaddress[3] = 0xFF;
            RSC_EPaddress[4] = 0x0F;
        }
    }
    else if(R_W_10_Vars.LBA == ROOT_SECT_ADDR){ // If PC is reading the Root Sector.
            if(ByteOfSect == 0) USB_ROMCopy((const uint8_t*)(&ROOT),RSC_EPaddress,64);
    }
    else if(R_W_10_Vars.LBA >= DATA_SECT_ADDR){ // If PC is reading the Data Sector.
        if(R_W_10_Vars.LBA == FILE_SECT_ADDR){ // If PC is reading ABOUT file data.
            if(ByteOfSect == 0) USB_ROMCopy(FILE_DATA,RSC_EPaddress,sizeof(FILE_DATA));
        }
    }
#else
    USB_RAMSet(0,MSD_SectData,512); // Blank Regions of memory are read as zero.

    if(R_W_10_Vars.LBA == BOOT_SECT_ADDR){  // If PC is reading the Boot Sector.
        USB_ROMCopy((const uint8_t*)(&BOOT16),MSD_SectData,sizeof(BOOT16));
        MSD_SectData[510] = 0x55;
        MSD_SectData[511] = 0xAA;
    }
    else if(R_W_10_Vars.LBA == FAT_SECT_ADDR){ // If PC is reading the FAT.
        MSD_SectData[0] = 0xF8;
        MSD_SectData[1] = 0xFF;
        MSD_SectData[2] = 0xFF;
        MSD_SectData[3] = 0xFF;
        MSD_SectData[4] = 0x0F;
    }
    else if(R_W_10_Vars.LBA == ROOT_SECT_ADDR){ // If PC is reading the Root Sector.
            USB_ROMCopy((const uint8_t*)(&ROOT),MSD_SectData,64);
    }
    else if(R_W_10_Vars.LBA >= DATA_SECT_ADDR){ // If PC is reading the Data Sector.
        if(R_W_10_Vars.LBA == FILE_SECT_ADDR){ // If PC is reading ABOUT file data.
            USB_ROMCopy(FILE_DATA,MSD_SectData,sizeof(FILE_DATA));
        }
    }
#endif
}

void MSD_TxSector(void){

}