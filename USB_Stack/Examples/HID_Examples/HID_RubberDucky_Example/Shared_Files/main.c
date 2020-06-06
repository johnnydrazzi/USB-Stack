/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * HID RubberDucky Example.
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

#include <xc.h>
#include <stdint.h>
#include "fuses.h"
#include "config.h"
#include "../../../../USB/usb.h"
#include "../../../../USB/usb_hid.h"
#include "../Shared_Files/usb_hid_reports.h"
#include "../Shared_Files/ASCII_2_KEY.h"

#if INTERRUPTS_MASK == 0
#error "RubberDucky Example needs usb interrupt method, because blocking is used."
#endif

static void example_init(void);
static void flash_led(void);
static void __interrupt() isr(void);
static void send_key(uint8_t modifier, uint8_t key_code);
static void send_consumer(uint8_t consumer_val);
static void print_keys(const uint8_t * str);

static const uint8_t message[] = "https://youtu.be/dQw4w9WgXcQ?t=43s\r";

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
    __delay_ms(2000);
    send_key(0,0);
    send_consumer(0x00);
    
    send_key(MOD_KEY_LEFTMETA, 0);
    __delay_ms(100);
    send_key(MOD_KEY_LEFTMETA, KEY_R);
    __delay_ms(100);
    send_key(0,0);
    __delay_ms(500);
    print_keys(message);
    while(1){}
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

static void send_key(uint8_t modifier, uint8_t key_code)
{
    g_hid_in_report1.Modifiers = modifier;
    g_hid_in_report1.Keycode   = key_code;
    hid_send_report(0);
    while(!g_hid_report_sent){}
}

static void send_consumer(uint8_t consumer_val)
{
    g_hid_in_report2.Consumer_Byte = consumer_val;
    hid_send_report(1);
    while(!g_hid_report_sent){}
}

static void print_keys(const uint8_t * str)
{
    uint8_t i = 0;
    while(str[i])
    {
        ascii_2_key(str[i]);
        send_key(g_key_result.Modifier, g_key_result.KeyCode);
        i++;
        if(str[i] == str[i - 1]) send_key(0,0);  // Repeated letters need to be released first.
    }
    send_key(0,0);
}

void usb_sof(void)
{
    //hid_service_sof(); // We ignore idle in this example
}

void USB_ServiceAppOut(void)
{
    
}

void hid_out(void)
{
    LED = g_hid_out_report1.CAPS_LOCK;
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    if(HID_EP_OUT_LAST_PPB == ODD) hid_arm_ep_out(HID_BD_OUT_EVEN);
    else hid_arm_ep_out(HID_BD_OUT_ODD);
    #else
    hid_arm_ep_out();
    #endif
}

static void __interrupt() isr(void)
{
    if(USB_INTERRUPT_ENABLE && USB_INTERRUPT_FLAG)
    {
        usb_tasks();
        USB_INTERRUPT_FLAG = 0;
    }
}