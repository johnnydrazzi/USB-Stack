/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 16/04/2023
 * 
 * HID Keyboard + Consumer Example.
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
 * PIC18FX6J53: 2000-FBFF.
 * PIC18FX7J53: 2000-1FBFF.
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
#include "../../../../Hardware/fuses.h"
#include "../../../../Hardware/config.h"
#include "../../../../USB/usb.h"
#include "../../../../USB/usb_hid.h"
#include "../Shared_Files/usb_hid_reports.h"
#include "../Shared_Files/ASCII_2_KEY.h"

#define BUTTON_WAS_PRESSED  (BUTTON_PRESSED && m_released)
#define BUTTON_WAS_RELEASED (BUTTON_RELEASED && !m_released)

static void example_init(void);
static void flash_led(void);
static void __interrupt() isr(void);
static void send_key(uint8_t modifier, uint8_t key_code);
static void send_consumer(uint8_t consumer_val);
static void service_reports_to_send(void);

static const uint8_t m_message[] = "Hello World!\r";
static bool m_released = true;
static bool m_send_report0 = false, m_send_report1 = false;

void main(void)
{
    example_init();
    flash_led();
    
    usb_init();
    
    // Comment out the following for polling method.
    INTCONbits.PEIE = 1;
    USB_INTERRUPT_FLAG = 0;
    USB_INTERRUPT_ENABLE = 1;
    INTCONbits.GIE = 1;
    
    send_key(0,0);
    send_consumer(0x00);
    
    uint8_t i = 0;
    while(1)
    {
        // Uncomment out the following for polling method.
        //usb_tasks(); 
        
        if(usb_get_state() != STATE_CONFIGURED) continue;  
        
        service_reports_to_send();
        
        // Uncomment the following for Keyboard Example
        if(g_hid_sent_report[0] == true && g_hid_report_sent){
            if(BUTTON_WAS_PRESSED){
                m_released = false;
                ascii_2_key(m_message[i]);
                send_key(g_key_result.Modifier, g_key_result.KeyCode);

            }
            else if(BUTTON_WAS_RELEASED)
            {
                m_released = true;
                send_key(0,0);
                i++;
                if(m_message[i] == 0) i = 0;
            }
        }
        
        // Uncomment the following for Consumer Example
//        if(g_hid_sent_report[1] == true && g_hid_report_sent){
//            if(BUTTON_WAS_PRESSED){
//                m_released = false;
//                send_consumer(_MUTE);
//            }
//            else if(BUTTON_WAS_RELEASED){
//                m_released = true;
//                send_consumer(0);
//            }
//        }
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
    
    #elif defined(_18F26J53) || defined(_18F27J53)
    LATB = 0;
    BUTTON_WPU |= (1 << BUTTON_WPU_BIT);
    BUTTON_RXPU_REG &= ~(1 << BUTTON_RXPU_BIT);
    
    #elif defined(_18F46J53) || defined(_18F47J53)
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

static void service_reports_to_send(void)
{
    if((g_hid_in_report_settings[0].Idle_Count_Overflow || m_send_report0) && g_hid_report_sent)
    {
        m_send_report0 = false;
        hid_send_report(0);
    }
    if((g_hid_in_report_settings[1].Idle_Count_Overflow || m_send_report1) && g_hid_report_sent)
    {
        m_send_report1 = false;
        hid_send_report(1);
    }
}

static void send_key(uint8_t modifier, uint8_t key_code)
{
    g_hid_in_report1.Modifiers = modifier;
    g_hid_in_report1.Keycode   = key_code;
    m_send_report0 = true;
}

static void send_consumer(uint8_t consumer_val)
{
    g_hid_in_report2.Consumer_Byte = consumer_val;
    m_send_report1 = true;
}

void usb_sof(void)
{
    hid_service_sof();
}

void hid_out(uint8_t report_num)
{
    if(g_hid_out_report1.CAPS_LOCK) LED_ON();
    else LED_OFF();
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    if(HID_EP_OUT_LAST_PPB == ODD) hid_arm_ep_out(HID_BD_OUT_EVEN);
    else hid_arm_ep_out(HID_BD_OUT_ODD);
    #else
    hid_arm_ep_out();
    #endif
}

static void __interrupt() isr(void)
{
    if(USB_INTERRUPT_ENABLE && USB_INTERRUPT_FLAG){
        usb_tasks();
        USB_INTERRUPT_FLAG = 0;
    }
}