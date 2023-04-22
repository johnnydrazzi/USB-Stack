/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 22/04/2023
 * 
 * MSD Simple Example.
 * Copyright (C) 2017-2023  John Izzard
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
 * offset of 0x2000. (For PIC16F145X offset is in words, therefore 0x1000).
 * 
 * If you are using the a J Series bootloader:
 * In the Option categories dropdown, select Memory Model. In the ROM ranges 
 * input, you need to put a range starting from the Codeoffset (0x2000) to 1KB from last 
 * byte in flash. e.g. For X7J53, 2000-1FBFF is used. This makes sure your code 
 * isn't placed in the same Flash Page as the Config Words. That area is write 
 * protected.
 * 
 * PIC18FX4J50: 2000-03BFF
 * PIC18FX5J50: 2000-07BFF
 * PIC18FX6J50: 2000-0FBFF
 * PIC18FX6J53: 2000-0FBFF
 * PIC18FX7J53: 2000-1FBFF
 * 
 * 2. DOWNLOAD FROM MPLABX
 * You can get MPLABX to download your code every time you press build. 
 * To set this up, right click on your MPLABX project, and select Properties. 
 * Under Conf: "PROCESSOR", click Building. Check the "Execute this line after 
 * build" box and place in this line of code (use the drive letter or name of 
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
 * turn on to indicate "bootloader mode" is active. If no program is present, 
 * just insert the USB cable.. Your PIC will now appear as a thumb drive.
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
 * and select send to PIC18F25K50 (for example). The bootloader will close and 
 * instantly start running your code. Alternatively, as seen in step two, you 
 * can get MPLABX to download the file automatically after a build.
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
#include "../../../Hardware/fuses.h"
#include "../../../Hardware/config.h"
#include "../../../USB/usb.h"
#include "../../../USB/usb_msd.h"

/** Boot Sector */
typedef struct
{
    uint8_t  jmpBoot[3];
    uint8_t  OEMName[8];
    uint16_t BytesPerSec;
    uint8_t  SecPerClus;
    uint16_t RsvdSecCnt;
    uint8_t  NumFATs;
    uint16_t RootEntCnt;
    uint16_t TotSec16;
    uint8_t  Media;
    uint16_t FATSz16;
    uint16_t SecPerTrk;
    uint16_t NumHeads;
    uint32_t HiddSec;
    uint32_t TotSec32;
    uint8_t  DrvNum;
    uint8_t  Reserved1;
    uint8_t  BootSig;
    uint8_t  VolID[4];
    uint8_t  VolLab[11];
    uint8_t  FilSysType[8];
}BOOT16_t;

static const BOOT16_t boot16 =
{
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
typedef struct
{
    uint8_t  Name[11];
    uint8_t  Attr;
    uint8_t  NTRes;
    uint8_t  CrtTimeTenth;
    uint16_t CrtTime;
    uint16_t CrtDate;
    uint16_t LstAccDate;
    uint16_t FstClusHI;
    uint16_t WrtTime;
    uint16_t WrtDate;
    uint16_t FstClusLO;
    uint32_t FileSize;
}DIR_ENTRY_t;

/** Root Directory Structure */
typedef struct
{
    DIR_ENTRY_t VOL;
    DIR_ENTRY_t FILE1;
}ROOT_DIR_t;

static const uint8_t file[] = "Hello World!";

/** Volume Root Entry */
static const ROOT_DIR_t root =
{
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
        sizeof(file)
    }
};

static void example_init(void);
static void flash_led(void);
static void __interrupt() isr(void);

void main(void)
{
    example_init();
    flash_led();
    
    usb_init();
    INTCONbits.PEIE = 1;
    USB_INTERRUPT_FLAG = 0;
    USB_INTERRUPT_ENABLE = 1;
    INTCONbits.GIE = 1;
    while(usb_get_state() != STATE_CONFIGURED){}
    while(1)
    {
        //usb_tasks();
        msd_tasks();
    }
    
    return;
}

static void __interrupt() isr(void)
{
    if(USB_INTERRUPT_ENABLE && USB_INTERRUPT_FLAG)
    {
        usb_tasks();
        USB_INTERRUPT_FLAG = 0;
    }
}

static void example_init(void)
{
    // Oscillator Settings.
    // PIC16F145X.
    #if defined(_PIC14E)
    #if XTAL_USED == NO_XTAL
    OSCCONbits.IRCF = 0xF;
    #endif
    #if XTAL_USED != MHz_12
    OSCCONbits.SPLLMULT = 1;
    #endif
    OSCCONbits.SPLLEN = 1;
    PLL_STARTUP_DELAY();
    #if XTAL_USED == NO_XTAL
    ACTCONbits.ACTSRC = 1;
    ACTCONbits.ACTEN = 1;
    #endif

    // PIC18FX450, PIC18FX550, and PIC18FX455.
    #elif defined(_18F4450_FAMILY_) || defined(_18F4550_FAMILY_)
    PLL_STARTUP_DELAY();
    
    // PIC18F14K50.
    #elif defined(_18F13K50) || defined(_18F14K50)
    OSCTUNEbits.SPLLEN = 1;
    PLL_STARTUP_DELAY();
    
    // PIC18F2XK50.
    #elif defined(_18F24K50) || defined(_18F25K50) || defined(_18F45K50)
    #if XTAL_USED == NO_XTAL
    OSCCONbits.IRCF = 7;
    #endif
    #if (XTAL_USED != MHz_12)
    OSCTUNEbits.SPLLMULT = 1;
    #endif
    OSCCON2bits.PLLEN = 1;
    PLL_STARTUP_DELAY();
    #if XTAL_USED == NO_XTAL
    ACTCONbits.ACTSRC = 1;
    ACTCONbits.ACTEN = 1;
    #endif

    // PIC18F2XJ53 and PIC18F4XJ53.
    #elif defined(__J_PART)
    OSCTUNEbits.PLLEN = 1;
    PLL_STARTUP_DELAY();
    #endif

    
    // Make boot pin digital.
    #if defined(BUTTON_ANSEL) 
    BUTTON_ANSEL &= ~(1<<BUTTON_ANSEL_BIT);
    #elif defined(BUTTON_ANCON)
    BUTTON_ANCON |= (1<<BUTTON_ANCON_BIT);
    #endif


    // Apply pull-up.
    #ifdef BUTTON_WPU
    #if defined(_PIC14E)
    WPUA = 0;
    #if defined(_16F1459)
    WPUB = 0;
    #endif
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    OPTION_REGbits.nWPUEN = 0;
    
    #elif defined(_18F4450_FAMILY_) || defined(_18F4550_FAMILY_)
    LATB = 0;
    LATD = 0;
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    #if BUTTON_RXPU_REG == INTCON2
    INTCON2 &= 7F;
    #else
    PORTE |= 80;
    #endif
    
    #elif defined(_18F13K50) || defined(_18F14K50)
    WPUA = 0;
    WPUB = 0;
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    INTCON2bits.nRABPU = 0;
    
    #elif defined(_18F24K50) || defined(_18F25K50) || defined(_18F45K50)
    WPUB = 0;
    TRISE &= 0x7F;
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    INTCON2bits.nRBPU = 0;
    
    #elif defined(_18F24J50) || defined(_18F25J50) || defined(_18F26J50) || defined(_18F26J53) || defined(_18F27J53)
    LATB = 0;
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    BUTTON_RXPU_REG &= ~(1 << BUTTON_RXPU_BIT);
    
    #elif defined(_18F44J50) || defined(_18F45J50) || defined(_18F46J50) || defined(_18F46J53) || defined(_18F47J53)
    LATB = 0;
    LATD = 0;
    LATE = 0;
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    BUTTON_RXPU_REG &= ~(1 << BUTTON_RXPU_BIT);
    #endif
    #endif
    
    LED_OFF();
    LED_OUPUT();
}

static void flash_led(void)
{
    for(uint8_t i = 0; i < 3; i++)
    {
        LED_ON();
        __delay_ms(500);
        LED_OFF();
        __delay_ms(500);
    }
}

void msd_rx_sector(void)
{
    #ifdef MSD_LIMITED_RAM
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    uint8_t *RSC_EP_ADDRESS;
    
    if(g_usb_ep_stat[MSD_EP][IN].Last_PPB == ODD) RSC_EP_ADDRESS = g_msd_ep_in_odd;
    else RSC_EP_ADDRESS = g_msd_ep_in_even;
    #else
    #define RSC_EP_ADDRESS g_msd_ep_in
    #endif
    
    usb_ram_set(0, RSC_EP_ADDRESS, 64);
    
    if(g_msd_rw_10_vars.LBA == BOOT_SECT_ADDR)  // If PC is reading the Boot Sector.
    {
        if(g_msd_byte_of_sect == 0) usb_rom_copy((const uint8_t*)(&boot16), RSC_EP_ADDRESS, sizeof(boot16));
        else if(g_msd_byte_of_sect == 448)
        {
            RSC_EP_ADDRESS[62] = 0x55;
            RSC_EP_ADDRESS[63] = 0xAA;
        }
    }
    else if(g_msd_rw_10_vars.LBA == FAT_SECT_ADDR) // If PC is reading the FAT.
    {
        if(g_msd_byte_of_sect == 0)
        {
            RSC_EP_ADDRESS[0] = 0xF8;
            RSC_EP_ADDRESS[1] = 0xFF;
            RSC_EP_ADDRESS[2] = 0xFF;
            RSC_EP_ADDRESS[3] = 0xFF;
            RSC_EP_ADDRESS[4] = 0x0F;
        }
    }
    else if(g_msd_rw_10_vars.LBA == ROOT_SECT_ADDR) // If PC is reading the Root Sector.
    {
        if(g_msd_byte_of_sect == 0) usb_rom_copy((const uint8_t*)(&root), RSC_EP_ADDRESS, 64);
    }
    else if(g_msd_rw_10_vars.LBA >= DATA_SECT_ADDR) // If PC is reading the Data Sector.
    {
        if(g_msd_rw_10_vars.LBA == FILE_SECT_ADDR) // If PC is reading ABOUT file data.
        {
            if(g_msd_byte_of_sect == 0) usb_rom_copy(file, RSC_EP_ADDRESS, sizeof(file));
        }
    }
    #else
    usb_ram_set(0, g_msd_sect_data, 512); // Blank Regions of memory are read as zero.

    if(g_msd_rw_10_vars.LBA == BOOT_SECT_ADDR)  // If PC is reading the Boot Sector.
    {
        usb_rom_copy((const uint8_t*)(&boot16), g_msd_sect_data, sizeof(boot16));
        g_msd_sect_data[510] = 0x55;
        g_msd_sect_data[511] = 0xAA;
    }
    else if(g_msd_rw_10_vars.LBA == FAT_SECT_ADDR) // If PC is reading the FAT.
    {
        g_msd_sect_data[0] = 0xF8;
        g_msd_sect_data[1] = 0xFF;
        g_msd_sect_data[2] = 0xFF;
        g_msd_sect_data[3] = 0xFF;
        g_msd_sect_data[4] = 0x0F;
    }
    else if(g_msd_rw_10_vars.LBA == ROOT_SECT_ADDR) // If PC is reading the Root Sector.
    {
        usb_rom_copy((const uint8_t*)(&root), g_msd_sect_data, 64);
    }
    else if(g_msd_rw_10_vars.LBA >= DATA_SECT_ADDR) // If PC is reading the Data Sector.
    {
        if(g_msd_rw_10_vars.LBA == FILE_SECT_ADDR) // If PC is reading ABOUT file data.
        {
            usb_rom_copy(file, g_msd_sect_data, sizeof(file));
        }
    }
    #endif
}

void msd_tx_sector(void)
{

}