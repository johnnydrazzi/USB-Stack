/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * MSD Internal Example.
 * Copyright (C) 2017-2024 John Izzard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <xc.h>
#include <stdint.h>
#include "fuses.h"
#include "config.h"
#include "usb.h"
#include "usb_msd.h"
#include "flash.h"

#if _HTC_EDITION_ == 0
#error "Use optimization level 2 and above."
#endif

#if defined(_PIC14E)
/* PIC16F145X ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x2000 (8KB) *Only 4KB usable due to 14-bit size words
    0x03FFF |______________|
    0x10000 |              |
            | CONFIG WORDS | 0x0E
    0x1000D |______________|
 */

                
#define FLASH_SPACE_START     0x01000 // Put ROM range as 0-7FF,800-FFF
#define END_OF_FLASH          0x02000

#define BOOT_START            (FLASH_SPACE_START)
#define SIGNATURE_WORD_START  0x11FE
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     0x1200
#define FAT_DEFAULT_SIZE      4
#define FAT_REST_OF_START     0x1204
#define ROOT_ENTRY_START      0x1400

#elif defined(_18F2450) || defined(_18F4450)
/* PIC18FX450 ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x2000 (8KB)
    0x03FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */
 
#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
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

#elif defined(_18F2455) || defined(_18F4455) || defined(_18F2458) || defined(_18F4458)
/* PIC18FX455 and PIC18FX458 ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x4000 (16KB)
    0x05FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */
 
#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
#define END_OF_FLASH          0x06000

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

#elif defined(_18F2550) || defined(_18F4550) || defined(_18F2553) || defined(_18F4553)
/* PIC18FX550 and PIC18FX553 ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x6000 (24KB)
    0x07FFF |______________|
   0x300000 |              |
            | CONFIG WORDS | 0x0E
   0x30000D |______________|
 */
 
#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
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

#elif defined(_18F14K50)
/* PIC18F14K50 ROM Space
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
           
#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
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
/* PIC18F24K50 ROM Space
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

#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
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
/* PIC18FX5K50 ROM Space
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


#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
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

#elif defined(_18F24J50) || defined(_18F44J50)
/* PIC18FX4J50 ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x01C00 (7KB)
    0x03BFF |______________|
    0x03C00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x03FFF |______________|
 */

#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
#define END_OF_FLASH          0x03C00

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (FLASH_SPACE_START + 1024)
#define FAT_DEFAULT_SIZE      4
#define ROOT_ENTRY_START      (FLASH_SPACE_START + 2048)

#elif defined(_18F25J50) || defined(_18F45J50)
/* PIC18FX5J50 ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x05C00 (23KB)
    0x07BFF |______________|
    0x07C00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x07FFF |______________|
 */
 
#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
#define END_OF_FLASH          0x07C00

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (FLASH_SPACE_START + 1024)
#define FAT_DEFAULT_SIZE      4
#define ROOT_ENTRY_START      (FLASH_SPACE_START + 2048)

#elif defined(_18F26J50) || defined(_18F46J50)
/* PIC18FX6J50 ROM Space
             ______________
    0x00000 |____RESET_____|
    0x00008 |____INT_HP____|
    0x00018 |____INT_LP____| 0x2000 (8KB)
            |              |
            |     CODE     |
    0x01FFF |______________|
    0x02000 |              |
            |  PROG MEM    | 0x0DC00 (55KB)
    0x0FBFF |______________|
    0x0FC00 |              |
            | CONFIG WORDS | 0x400 (1KB)
    0x0FFFF |______________|
 */
 
#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
#define END_OF_FLASH          0x0FC00

#define BOOT_START            FLASH_SPACE_START
#define BOOT_SIZE             62
#define BOOTSTRAP_START       (FLASH_SPACE_START + BOOT_SIZE)
#define BOOTSTRAP_SIZE        448
#define SIGNATURE_WORD_START  (BOOTSTRAP_START + BOOTSTRAP_SIZE)
#define SIGNATURE_WORD_SIZE   2
#define FAT_DEFAULT_START     (FLASH_SPACE_START + 1024)
#define FAT_DEFAULT_SIZE      4
#define ROOT_ENTRY_START      (FLASH_SPACE_START + 2048)

#elif defined(_18F26J53) || defined(_18F46J53)
/* PIC18FX6J53 ROM Space
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

#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
#define END_OF_FLASH          0x0FC00

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
/* PIC18FX7J53 ROM Space
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

#define FLASH_SPACE_START     0x02000 // Put ROM range as 0-1FFF
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
 * Using our format method, we get 20.5KB volume.
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
#ifdef USE_BOOT_LED
static void flash_led(void);
#endif
static uint32_t LBA_to_flash_addr(uint32_t LBA);
static void __interrupt() isr(void);

void main(void)
{
    example_init();
    #ifdef USE_BOOT_LED
    LED_OFF();
    LED_OUPUT();
    flash_led();
    #endif
    
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
}

#ifdef USE_BOOT_LED
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
#endif

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
        uint8_t *p_ep = MSD_EP_IN_LAST_PPB == ODD ? g_msd_ep_in_odd : g_msd_ep_in_even;
        
        Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, buffer);
        for(i = 0, x = 0; i < 64; i += 2, x++) p_ep[x] = buffer[i];
        Flash_ReadBytes((uint24_t)(addr + 32 + g_msd_byte_of_sect), 64, buffer);
        for(i = 0, x = 32; i < 64; i += 2, x++) p_ep[x] = buffer[i];
        
        #else
        #ifdef MSD_LIMITED_RAM
        if(MSD_EP_IN_LAST_PPB == ODD) Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, g_msd_ep_in_odd);
        else Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, g_msd_ep_in_even);
        #else
        Flash_ReadBytes((uint24_t)addr, 512, g_msd_sect_data);
        #endif
        #endif
    }
    else
    {   
        #ifdef MSD_LIMITED_RAM
        if(MSD_EP_IN_LAST_PPB == ODD) usb_ram_set(0, g_msd_ep_in_odd, 64);
        else usb_ram_set(0, g_msd_ep_in_even, 64);
        #else
        usb_ram_set(0, g_msd_sect_data, 512);
        #endif
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
        #ifdef MSD_LIMITED_RAM
        Flash_ReadBytes((uint24_t)(addr + g_msd_byte_of_sect), 64, g_msd_ep_in);
        #else
        Flash_ReadBytes((uint24_t)addr, 512, g_msd_sect_data);
        #endif
        #endif
    }
    #ifdef MSD_LIMITED_RAM
    else usb_ram_set(0, g_msd_ep_in, 64);
    #else
    else usb_ram_set(0, g_msd_sect_data, 512);
    #endif
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
        uint8_t *p_ep = MSD_EP_OUT_LAST_PPB == ODD ? g_msd_ep_out_odd : g_msd_ep_out_even;
        
        for(i = 0, x = 0; i < 64; i += 2, x++)
        {
            buffer[i] = p_ep[x];
            buffer[i + 1] = 0xFF;
        }
        Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), buffer);
        for(i = 0, x = 32; i < 64; i += 2, x++)
        {
            buffer[i] = p_ep[x];
            buffer[i + 1] = 0xFF;
        }
        Flash_EraseWriteBlock((uint24_t)(addr + 32 + g_msd_byte_of_sect), buffer);
        
        #elif defined(__J_PART)
        #ifdef MSD_LIMITED_RAM
        if(MSD_EP_OUT_LAST_PPB == ODD) Flash_WriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_odd);
        else Flash_WriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_even);
        #else
        for(uint16_t i = 0; i < 512; i += 64, addr += 64) Flash_WriteBlock((uint24_t)addr, g_msd_sect_data + i);
        #endif
        #else

        #ifdef MSD_LIMITED_RAM
        if(MSD_EP_OUT_LAST_PPB == ODD) Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_odd);
        else Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out_even);
        #else
        for(uint16_t i = 0; i < 512; i += _FLASH_ERASE_SIZE, addr += _FLASH_ERASE_SIZE) Flash_EraseWriteBlock((uint24_t)addr, g_msd_sect_data + i);
        #endif
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
        #ifdef MSD_LIMITED_RAM
        Flash_WriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out);
        #else
        for(uint16_t i = 0; i < 512; i += 64, addr += 64) Flash_WriteBlock((uint24_t)addr, g_msd_sect_data + i);
        #endif
        
        #else
        #ifdef MSD_LIMITED_RAM
        Flash_EraseWriteBlock((uint24_t)(addr + g_msd_byte_of_sect), g_msd_ep_out);
        #else
        for(uint16_t i = 0; i < 512; i += _FLASH_ERASE_SIZE, addr += _FLASH_ERASE_SIZE) Flash_EraseWriteBlock((uint24_t)addr, g_msd_sect_data + i);
        #endif
        #endif
        #endif
    }
}

static uint32_t LBA_to_flash_addr(uint32_t LBA)
{
    return (LBA * BYTES_PER_BLOCK_LE) + FLASH_SPACE_START;
}