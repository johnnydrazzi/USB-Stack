/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * MSD Internal Example.
 * Copyright (C) 2017-2020  John Izzard
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
#include <pic18.h>
#include "fuses.h"
#include "config.h"
#include "../../../../USB/usb.h"
#include "../../../../USB/usb_msd.h"
#include "../Shared_Files/flash.h"

#if !defined(MSD_LIMITED_RAM) && !defined(__J_PART)
#error "Must use MSD_LIMITED_RAM option for this example."
#endif

#if defined(_PIC14E)
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

#define BOOT_START            (FLASH_SPACE_START)
#define SIGNATURE_WORD_START  0x11FE
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     0x1200
#define FAT_DEFAULT_SIZE      4
#define FAT_REST_OF_START     0x1204
#define ROOT_ENTRY_START      0x1400


#elif defined(_18F14K50)
/* PIC18F14K50 ROM Space (Free Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2800 (10KB)
            |              |
            |     CODE     |
    0x027FF |______________|
    0x02800 |              |
            | FLASH SPACE  | 0x1800 (6KB) 
    0x03FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */

/* PIC18F14K50 ROM Space (Std/Pro Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            | FLASH SPACE  | 0x6000 (8KB) 
    0x03FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */

#if _HTC_EDITION_ == 0 // Free Version - put ROM range as 0-27FF
#define FLASH_SPACE_START     0x02800
#else                  // Standard/Pro Version - put ROM range as 0-1FFF
#define FLASH_SPACE_START     0x02000
#endif
#define END_OF_FLASH          0x04000

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (SIGNATURE_WORD_START + SIGNATURE_WORD_SIZE)
#define FAT_DEFAULT_SIZE      4
#define FAT_REST_OF_START     (FAT_DEFAULT_START + FAT_DEFAULT_SIZE)
#define FAT_REST_OF_SIZE      (512 - FAT_DEFAULT_SIZE)
#define ROOT_ENTRY_START      (FAT_REST_OF_START + FAT_REST_OF_SIZE)


#elif defined(_18F24K50)
/* PIC18F24K50 ROM Space (Free Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2800 (10KB)
            |              |
            |     CODE     |
    0x027FF |______________|
    0x02800 |              |
            | FLASH SPACE  | 0x1800 (6KB) 
    0x03FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */

/* PIC18F24K50 ROM Space (Std/Pro Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            | FLASH SPACE  | 0x6000 (8KB) 
    0x03FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */

#if _HTC_EDITION_ == 0 // Free Version - put ROM range as 0-27FF
#define FLASH_SPACE_START     0x02800
#else                  // Standard/Pro Version - put ROM range as 0-1FFF
#define FLASH_SPACE_START     0x02000
#endif
#define END_OF_FLASH          0x04000

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (SIGNATURE_WORD_START + SIGNATURE_WORD_SIZE)
#define FAT_DEFAULT_SIZE      4
#define FAT_REST_OF_START     (FAT_DEFAULT_START + FAT_DEFAULT_SIZE)
#define FAT_REST_OF_SIZE      (512 - FAT_DEFAULT_SIZE)
#define ROOT_ENTRY_START      (FAT_REST_OF_START + FAT_REST_OF_SIZE)


#elif defined(_18F25K50) || defined(_18F45K50)
/* PIC18FX5K50 ROM Space (Free Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2800 (10KB)
            |              |
            |     CODE     |
    0x027FF |______________|
    0x02800 |              |
            | FLASH SPACE  | 0x5800 (22KB) 
    0x07FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */

/* PIC18FX5K50 ROM Space (Std/Pro Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            | FLASH SPACE  | 0x6000 (24KB) 
    0x07FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */

#if _HTC_EDITION_ == 0 // Free Version - put ROM range as 0-27FF
#define FLASH_SPACE_START     0x02800
#else                  // Standard/Pro Version - put ROM range as 0-1FFF
#define FLASH_SPACE_START     0x02000
#endif
#define END_OF_FLASH          0x08000

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (SIGNATURE_WORD_START + SIGNATURE_WORD_SIZE)
#define FAT_DEFAULT_SIZE      4
#define FAT_REST_OF_START     (FAT_DEFAULT_START + FAT_DEFAULT_SIZE)
#define FAT_REST_OF_SIZE      (512 - FAT_DEFAULT_SIZE)
#define ROOT_ENTRY_START      (FAT_REST_OF_START + FAT_REST_OF_SIZE)


#elif defined(_18F26J53) || defined(_18F46J53)
/* PIC18FX6J53 ROM Space (Free Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2800 (10KB)
            |              |
            |     CODE     |
    0x027FF |______________|
    0x02800 |              |
            | FLASH SPACE  | 0xD400 (53KB) 
    0x0FBFF |______________|
    0x0FC00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x10000 |______________|
 */

/* PIC18FX6J53 ROM Space (Std/Pro Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            | FLASH SPACE  | 0xDC00 (55KB) 
    0x0FBFF |______________|
    0x0FC00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x10000 |______________|
 */

#if _HTC_EDITION_ == 0 // Free Version - put ROM range as 0-27FF
#define FLASH_SPACE_START     0x02800
#else                  // Standard/Pro Version - put ROM range as 0-1FFF
#define FLASH_SPACE_START     0x02000
#endif
#define END_OF_FLASH          0x1FC00

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (FLASH_SPACE_START + 1024)
#define FAT_DEFAULT_SIZE      4
#define ROOT_ENTRY_START      (FLASH_SPACE_START + 2048)


#elif defined(_18F27J53) || defined(_18F47J53)
/* PIC18FX7J53 ROM Space (Free Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2800 (10KB)
            |              |
            |     CODE     |
    0x027FF |______________|
    0x02800 |              |
            | FLASH SPACE  | 0x1D400 (117KB) 
    0x1FBFF |______________|
    0x1FC00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x20000 |______________|
 */

/* PIC18FX7J53 ROM Space (Std/Pro Compiler)
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            | FLASH SPACE  | 0x1DC00 (119KB) 
    0x1FBFF |______________|
    0x1FC00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x20000 |______________|
 */

#if _HTC_EDITION_ == 0 // Free Version - put ROM range as 0-27FF
#define FLASH_SPACE_START     0x02800
#else                  // Standard/Pro Version - put ROM range as 0-1FFF
#define FLASH_SPACE_START     0x02000
#endif
#define END_OF_FLASH          0x1FC00

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (FLASH_SPACE_START + 1024)
#define FAT_DEFAULT_SIZE      4
#define ROOT_ENTRY_START      (FLASH_SPACE_START + 2048)
#endif

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

static const BOOT16_t boot16 __at(BOOT_START) =
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
    {'F','A','T','1','2',' ',' ',' '}
};

static const uint8_t signature_word[SIGNATURE_WORD_SIZE] __at(SIGNATURE_WORD_START) = {0x55, 0xAA};
static const uint8_t fat12_default[FAT_DEFAULT_SIZE]     __at(FAT_DEFAULT_START)    = {0xF8,0xFF,0xFF,0x00};

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

static const DIR_ENTRY_t dir_entry __at(ROOT_ENTRY_START) =
{
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

static void example_init(void);
static void flash_led(void);
static uint32_t LBA_to_flash_addr(uint32_t LBA);
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
    #if defined(_PIC14E)
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

    #elif defined(_18F14K50) || defined(_18F13K50)
    OSCTUNEbits.SPLLEN = 1;
    PLL_STARTUP_DELAY();
    ANSEL = 0;
    
    #elif defined(_18F24K50) || defined(_18F25K50) || defined(_18F45K50)
    #if XTAL_USED == NO_XTAL
    OSCCONbits.IRCF = 7;
    #endif
    #if (XTAL_USED == MHz_16) || (XTAL_USED == NO_XTAL)
    OSCTUNEbits.SPLLMULT = 1;
    #endif
    OSCCON2bits.PLLEN = 1;
    PLL_STARTUP_DELAY();
    #if XTAL_USED == NO_XTAL
    ACTCONbits.ACTSRC = 1;
    ACTCONbits.ACTEN = 1;
    ANSELA = 0;
    #endif
    ANSELB = 0;
    
    #elif defined(_18F26J53) || defined(_18F46J53) || defined(_18F27J53) || defined(_18F47J53)
    OSCTUNEbits.PLLEN = 1;
    PLL_STARTUP_DELAY();
    #endif

    
    LED = 0;
    LED_TRIS = 0; 
}

static void flash_led(void)
{
    for(uint8_t i = 0; i < 3; i++)
    {
        LED = 1;
        __delay_ms(500);
        LED = 0;
        __delay_ms(500);
    }
}

void msd_rx_sector(void)
{
    uint32_t addr;
    #if defined(_PIC14E)
    uint8_t buffer[64];
    uint8_t i, x;
    #endif
    addr = LBA_to_flash_addr(g_msd_rw_10_vars.LBA); // Convert from LBA address space to flash address space. 
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    if(addr < END_OF_FLASH) // If address is in flash space.
    {
        #if defined(_PIC14E)
        #error "Double buffering is not yet supported for PIC16 in this example, just not enough space." 
//        uint8_t *p_ep;
//        if(MSD_EP_IN_LAST_PPB == ODD) p_ep = g_msd_ep_in_odd;
//        else p_ep = g_msd_ep_in_even;
//        
//        Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, buffer);
//        for(i = 0, x = 0; i < 64; i += 2, x++) p_ep[x] = buffer[i];
//        Flash_ReadBytes((uint24_t)(addr + 32 + g_msd_byte_of_sect), 64, buffer);
//        for(i = 0, x = 32; i < 64; i += 2, x++) p_ep[x] = buffer[i];
        
        #else
        if(MSD_EP_IN_LAST_PPB == ODD) Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, g_msd_ep_in_odd);
        else Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, g_msd_ep_in_even);
        #endif
    }
    else
    {
        if(MSD_EP_IN_LAST_PPB == ODD) usb_ram_set(0, g_msd_ep_in_odd, 64);
        else usb_ram_set(0, g_msd_ep_in_even, 64);
    }
    #else
    if(addr < END_OF_FLASH)
    {
        #if defined(_PIC14E)
        Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, buffer);
        for(i = 0, x = 0; i < 64; i += 2, x++) g_msd_ep_in[x] = buffer[i];
        Flash_ReadBytes((uint24_t)(addr + 32 + g_msd_byte_of_sect), 64, buffer);
        for(i = 0, x = 32; i < 64; i += 2, x++) g_msd_ep_in[x] = buffer[i];
        
        #else
        Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, g_msd_ep_in);
        #endif
    }
    else usb_ram_set(0, g_msd_ep_in, 64);
    #endif
}

void msd_tx_sector(void)
{
    uint32_t addr;
    #if defined(_PIC14E)
    uint8_t buffer[64];
    uint8_t i, x;
    #endif
    addr = LBA_to_flash_addr(g_msd_rw_10_vars.LBA); // Convert from LBA address space to flash address space.
    if(addr < END_OF_FLASH) // If address is in flash space.
    {
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        #if defined(_PIC14E)
        // NOT YET SUPPORTED
//        uint8_t *p_ep;
//        if(MSD_EP_OUT_LAST_PPB == ODD) p_ep = g_msd_ep_out_odd;
//        else p_ep = g_msd_ep_out_even;
//        
//        for(i = 0, x = 0; i < 64; i += 2, x++)
//        {
//            buffer[i] = p_ep[x];
//            buffer[i + 1] = 0xFF;
//        }
//        Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), buffer);
//        for(i = 0, x = 32; i < 64; i += 2, x++)
//        {
//            buffer[i] = p_ep[x];
//            buffer[i + 1] = 0xFF;
//        }
//        Flash_EraseWriteBlock((uint24_t)(addr + 32 + g_msd_byte_of_sect), buffer);
        
        #elif defined(__J_PART)
        if(MSD_EP_OUT_LAST_PPB == ODD) Flash_WriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_odd);
        else Flash_WriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_even);
        
        #else
        if(MSD_EP_OUT_LAST_PPB == ODD) Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_odd);
        else Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_even);
        #endif

        #else
        #if defined(_PIC14E)
        for(i = 0, x = 0; i < 64; i += 2, x++)
        {
            buffer[i] = g_msd_ep_out[x];
            buffer[i + 1] = 0xFF;
        }
        Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), buffer);
        for(i = 0, x = 32; i < 64; i += 2, x++)
        {
            buffer[i] = g_msd_ep_out[x];
            buffer[i + 1] = 0xFF;
        }
        Flash_EraseWriteBlock((uint24_t)(addr + 32 + g_msd_byte_of_sect), buffer);
        
        #elif defined(__J_PART)
        Flash_WriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out);
        
        #else
        Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out);
        #endif
        #endif
    }
}

static uint32_t LBA_to_flash_addr(uint32_t LBA)
{
    return (LBA * BYTES_PER_BLOCK_LE) + FLASH_SPACE_START;
}