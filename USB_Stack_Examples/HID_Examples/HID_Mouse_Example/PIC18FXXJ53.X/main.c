/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * HID Mouse Example (for PIC18FXXJ53).
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

#include <xc.h>
#include <stdint.h>
#include "fuses.h"
#include "config.h"
#include "../Shared_Files/USB.h"
#include "../Shared_Files/usb_hid.h"

#define BUTTON_WAS_PRESSED  (BUTTON && released)
#define BUTTON_WAS_RELEASED (!BUTTON && !released)

void flash_LED(void);
void interrupt ISR(void);
void ServiceReportsToSend(void);

bool released = true;
bool SendReport = false;

void main(void) {
    OSCTUNEbits.PLLEN = 1;
    PLL_STARTUP_DELAY();
    
    LED = 0;
    LED_TRIS = 0; // LED tris  
    
    flash_LED();
    
    // Comment out the following for polling method.
    USB_Init();
    INTCONbits.PEIE = 1;
    USB_Interrupt_Flag = 0;
    USB_Interrupt_Enable = 1;
    INTCONbits.GIE = 1;
    
    In_Report1.Array[0] = 0;
    In_Report1.Array[1] = 0;
    In_Report1.Array[2] = 0;
    SendReport = true;
    
    while(1){
        // Uncomment out the following for polling method.
        //USB_Tasks(); 
        
        if(USB_STATE != Configured) continue;
        
        ServiceReportsToSend();
        
        // Uncomment the following for Button Example
//        if(ReportSent){
//            if(BUTTON_WAS_PRESSED){
//                released = false;
//                In_Report1.data.BUTTON_1 = 1;
//                SendReport = true;
//            }
//            else if(BUTTON_WAS_RELEASED){
//                released = true;
//                In_Report1.Array[0] = 0;
//                SendReport = true;
//            }
//        }
        
        // Uncomment the following for Pointer Example
        if(ReportSent){
            if(BUTTON_WAS_PRESSED){
                released = false;
                In_Report1.data.y = -65;
                SendReport = true;
            }
            else if(BUTTON_WAS_RELEASED){
                released = true;
            }
        }
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

void ServiceReportsToSend(void){
    if((In_Report_Settings[0].Idle_Count_Overflow||SendReport) && ReportSent){
        SendReport = false;
        HID_SendReport(0);
    }
}

void USB_SOF(void){
    HID_ServiceSOF();
}

void interrupt ISR(void){
    if(USB_Interrupt_Enable && USB_Interrupt_Flag){
        USB_Tasks();
        USB_Interrupt_Flag = 0;
    }
}