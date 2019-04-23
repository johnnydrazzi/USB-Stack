/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 20/02/2019
 * 
 * CDC Serial UART Example (for PIC18FXXJ53).
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
#include <stdbool.h>
#include "fuses.h"
#include "config.h"
#include "../Shared_Files/USB.h"
#include "../Shared_Files/UART.h"
#include "../Shared_Files/usb_cdc.h"

void flash_LED(void);
void interrupt ISR(void);
void VCP_Tasks(void);

#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

bool RxSent = true;
bool TxRcv = false;

uint8_t RxBuffer[RX_BUFFER_SIZE];
uint8_t RxIndex = 0;
bool    RxBufferFull = false;

uint8_t TxBuffer[TX_BUFFER_SIZE];
uint8_t TxIndex = 0;
uint8_t Tx2cpy = 0;

uint8_t EP_Index;

void main(void){
    OSCTUNEbits.PLLEN = 1;
    PLL_STARTUP_DELAY();
            
    LED = 0;
    LED_TRIS = 0; // LED tris  
    
    flash_LED();
    
    UART_Init();
    USB_Init();
    INTCONbits.PEIE = 1;
    USB_Interrupt_Flag = 0;
    USB_Interrupt_Enable = 1;
    INTCONbits.GIE = 1;
    
    while(1){
        while(USB_STATE < Configured){} // Pause if not configured or suspended.
        VCP_Tasks();
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

void CDC_SetControlLineState(void){
#ifdef USE_RTS
    if(SetControlLineStateData.RTS_bit) USB_HAS_SET_RTS = true;
    else USB_HAS_SET_RTS = false;
    
    // If the buffer is not full and the computer wants to set RTS, set RTS.
    if((RxBufferFull == false) && USB_HAS_SET_RTS) RTS = RTS_ACTIVE;
    else RTS = RTS_ACTIVE ^ 1; 
#endif
#ifdef USE_DTR
    if(SetControlLineStateData.DTR_bit) DTR = DTR_ACTIVE;
    else DTR = DTR_ACTIVE ^ 1;
#endif
}

void CDC_SetLineCoding(void){
    uint16_t calc_SPBRG = (uint16_t)(((((float)_XTAL_FREQ/SetLineCodingData.dwDTERate)/4)-1)+0.5);
    SPBRG = (uint8_t)calc_SPBRG;
    SPBRGH = (uint8_t)(calc_SPBRG>>8);
}

void CDC_DataOUT(void){
    TxRcv = true;
}

void CDC_DataIN(void){
    RxSent = true;
}

void CDC_Notification(void){
#if defined(USE_DTR)||defined(USE_DCD)
    sentLastNotification = true;
#endif
}

void VCP_Tasks(void){
    // If UART packet received, add to RxBuffer.
    if(PIR1bits.RC1IF){
#ifdef USE_RTS
        if(RxIndex == RX_BUFFER_SIZE-5){ // If buffer is almost full
            RxBufferFull = true;
            RTS = RTS_ACTIVE ^ 1;
        }
#endif
        if(RxIndex < RX_BUFFER_SIZE) RxBuffer[RxIndex++] = UART_Read(); // If RxIndex is not out of bounds, read the Rx data and put in buffer.
    }                                                                   // If this is false (buffer is full) data will be lost.

    // If there is data in RxBuffer and USB is not busy, send it.
    if(RxSent && RxIndex){
        RxSent = false;
        USB_RAMCopy(RxBuffer,CDC_DAT_EP_IN,RxIndex);
        CDC_Arm_DAT_EP_IN(RxIndex);
        RxIndex = 0;
#ifdef USE_RTS
        if(USB_HAS_SET_RTS && (RTS != RTS_ACTIVE)) RTS = RTS_ACTIVE;
        RxBufferFull = false;
#endif
    }

    // If Tx Data was received on the USB endpoint, and UART currently has no data to send.
    if(TxRcv && (Tx2cpy == 0)){
        TxRcv = false;                        // Reset TxRcv.
        Tx2cpy = NumDataOut;                  // Number of bytes to grab from EP.
        TxIndex = 0;                          // Reset TxIndex.
        USB_RAMCopy(CDC_DAT_EP_OUT,TxBuffer,Tx2cpy); // Copy from endpoint to TxBuffer.
        CDC_Arm_DAT_EP_OUT();
    }

    // If there is data in TxBuffer, send it over UART.
    if(Tx2cpy){
#if defined(USE_RTS) && !defined(USE_DTR)
        if(CTS == CTS_ACTIVE){ // Block if CTS is not active.
            UART_Write(TxBuffer[TxIndex]);
            TxIndex++;
            Tx2cpy--;
        }
#elif !defined(USE_RTS) && defined(USE_DTR)
        if(DSR == DSR_ACTIVE){ // Block if DSR is not active.
            UART_Write(TxBuffer[TxIndex]);
            TxIndex++;
            Tx2cpy--;
        }
#elif defined(USE_RTS) && defined(USE_DTR)
        if(CTS == CTS_ACTIVE && DSR == DSR_ACTIVE){ // Block if either CTS or DSR is not active.
            UART_Write(TxBuffer[TxIndex]);
            TxIndex++;
            Tx2cpy--;
        }
#else
        UART_Write(TxBuffer[TxIndex]);
        TxIndex++;
        Tx2cpy--;
#endif
    }

#ifdef USE_DTR
    if((DSR ^ DSR_ACTIVE) == SERIAL_STATE_Data.bTxCarrier){ // Detect change on DSR.
        SERIAL_STATE_Data.bTxCarrier ^= 1;
        sendNotification = true;
    }
#endif
#ifdef USE_DCD
    if((DCD ^ DCD_ACTIVE) == SERIAL_STATE_Data.bRxCarrier){ // Detect change on DCD.
        SERIAL_STATE_Data.bRxCarrier ^= 1;
        sendNotification = true;
    }
#endif
#if defined(USE_DTR)||defined(USE_DCD)
    NotificationTasks();
#endif
}