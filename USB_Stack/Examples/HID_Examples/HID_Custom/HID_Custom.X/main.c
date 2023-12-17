/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 17/12/2023
 * 
 * HID Custom Example.
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

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../../../Hardware/fuses.h"
#include "../../../../Hardware/config.h"
#include "../../../../USB/usb.h"
#include "../../../../USB/usb_hid.h"
#include "usb_hid_reports.h"

typedef enum
{
    COMMAND_TOGGLE_LED = 0x80,
    COMMAND_GET_BUTTON_STATUS = 0x81,
    COMMAND_READ_POTENTIOMETER = 0x37
}hid_custom_example_commands_t;

static void example_init(void);
#ifdef USE_BOOT_LED
static void flash_led(void);
#endif
static void __interrupt() isr(void);

static bool m_out_event = false;

void main(void)
{
    example_init();
    
    // Setup analog pin.
    #if defined(_PIC14E) && !defined(_16F1454)
    ADCON0bits.CHS = 3; // AN3 (RA4).
    ADCON1bits.ADCS = 0b110;
    ADCON1bits.ADFM = 1;
    ADCON0bits.ADON = 1;
    #elif defined(_18F13K50) || defined(_18F14K50)
    ADCON0bits.CHS = 10; // AN10 (RB4).
    ADCON2bits.ADCS = 0b110;
    ADCON2bits.ACQT = 0b011;
    ADCON2bits.ADFM = 1;
    ADCON0bits.ADON = 1;
    #elif defined(_18F24K50) || defined(_18F25K50) || defined(_18F45K50)
    ADCON0bits.CHS = 0; // AN0 (RA0).
    ADCON2bits.ADCS = 0b110;
    ADCON2bits.ACQT = 0b011;
    ADCON2bits.ADFM = 1;
    ADCON0bits.ADON = 1;
    #elif defined(_18F4550_FAMILY_) || defined(_18F4450_FAMILY_)
    ADCON1bits.PCFG = 1; // RA0 analog pin
    ADCON0bits.CHS = 0; // AN0 (RA0).
    ADCON2bits.ADCS = 0b110;
    ADCON2bits.ACQT = 0b011;
    ADCON2bits.ADFM = 1;
    ADCON0bits.ADON = 1;
    #elif defined(__J_PART)
    ADCON0bits.CHS = 8; // AN8 (RB2).
    ADCON1bits.ADCS = 0b110;
    ADCON1bits.ACQT = 0b011;
    ADCON1bits.ADFM = 1;
    ADCON0bits.ADON = 1;
    #endif

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
        if(m_out_event)
        {
            switch(g_hid_out_report1.array[0])
            {
                case COMMAND_TOGGLE_LED:
				    #ifdef USE_BOOT_LED
                    LED_LAT ^= (1 << LED_BIT);
					#endif
                    break;
                case COMMAND_GET_BUTTON_STATUS:
                    if(g_hid_report_sent != true) break;

                    g_hid_in_report1.array[0] = COMMAND_GET_BUTTON_STATUS;
                    g_hid_in_report1.array[1] = BUTTON_PRESSED ? false : true;
                    hid_send_report(0);
                    break;
                case COMMAND_READ_POTENTIOMETER:
                    if(g_hid_report_sent != true) break;

                    #if !defined(_16F1454)
                    ADCON0bits.GO_nDONE = 1;
                    while(ADCON0bits.GO_nDONE){}
                    g_hid_in_report1.array[0] = COMMAND_READ_POTENTIOMETER;
                    #if defined(_18F47J53_FAMILY_) || defined(_18F2458) || \
                        defined(_18F4458) || defined(_18F2553) || defined(_18F4553)
                    // 12-bit ADC. If not converted to 10-bit number the
                    // application will report an exception.
                    g_hid_in_report1.array[1] = (ADRESH << 6) | (ADRESL >> 2); 
                    g_hid_in_report1.array[2] = ADRESH >> 2;
                    #else
                    g_hid_in_report1.array[1] = ADRESL;
                    g_hid_in_report1.array[2] = ADRESH;
                    #endif
                    #else
                    g_hid_in_report1.array[0] = COMMAND_READ_POTENTIOMETER;
                    g_hid_in_report1.array[1] = 0;
                    g_hid_in_report1.array[2] = 0;
                    #endif
                    hid_send_report(0);
                    break;
            }
            #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
            if(HID_EP_OUT_LAST_PPB == ODD) hid_arm_ep_out(HID_BD_OUT_EVEN);
            else hid_arm_ep_out(HID_BD_OUT_ODD);
            #else
            hid_arm_ep_out();
            #endif
            m_out_event = false;
        }
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

void hid_out(uint8_t report_num)
{
    m_out_event = true;
}

void usb_sof(void)
{
    //hid_service_sof(); // Idle rate is always infinite in this example, no point.
}

static void __interrupt() isr(void)
{
    if(USB_INTERRUPT_ENABLE && USB_INTERRUPT_FLAG)
    {
        usb_tasks();
        USB_INTERRUPT_FLAG = 0;
    }
}