/**
 * @file main.c
 * @brief Main C file.
 * @author John Izzard
 * @date 22/04/2023
 * 
 * CDC Serial UART Example.
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
#include "../../../Hardware/fuses.h"
#include "../../../Hardware/config.h"
#include "../../../USB/usb.h"
#include "UART.h"
#include "../../../USB/usb_cdc.h"

static void example_init(void);
#ifdef USE_BOOT_LED
static void flash_led(void);
#endif
static void __interrupt() isr(void);
static void vcp_tasks(void);

#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

static bool volatile m_serial_pkt_sent = true;
static bool volatile m_serial_pkt_rcv = false;

static uint8_t m_rx_buffer[RX_BUFFER_SIZE];
static uint8_t m_rx_index = 0;
static bool volatile m_rx_buffer_full = false;

static uint8_t m_tx_buffer[TX_BUFFER_SIZE];
static uint8_t m_tx_index = 0;
static uint8_t m_tx_to_cpy = 0;

void main(void)
{
    example_init();
    #ifdef USE_BOOT_LED
	LED_OFF();
    LED_OUPUT();
    flash_led();
	#endif
    
    UART_Init();
    
    // Make UART pins digital.
    #if defined(_18F13K50) || defined(_18F14K50)
    ANSELH = 0;
    #elif !defined(__J_PART)
    ANSELC = 0;
    #endif
    
    usb_init();
    INTCONbits.PEIE = 1;
    USB_INTERRUPT_FLAG = 0;
    USB_INTERRUPT_ENABLE = 1;
    INTCONbits.GIE = 1;
    
    while(1)
    {
        while(usb_get_state() < STATE_CONFIGURED){} // Pause if not configured or suspended.
        vcp_tasks();
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

void cdc_set_control_line_state(void)
{
    #ifdef USE_RTS
    if(g_cdc_set_control_line_state.RTS_bit) g_cdc_has_set_rts = true;
    else g_cdc_has_set_rts = false;
    
    // If the buffer is not full and the computer wants to set RTS, set RTS.
    if((m_rx_buffer_full == false) && g_cdc_has_set_rts) RTS = RTS_ACTIVE;
    else RTS = RTS_ACTIVE ^ 1; 
    #endif
    #ifdef USE_DTR
    if(g_cdc_set_control_line_state.DTR_bit) DTR = DTR_ACTIVE;
    else DTR = DTR_ACTIVE ^ 1;
    #endif
}

void cdc_set_line_coding(void)
{
    uint16_t calc_SPBRG = (uint16_t)((_XTAL_FREQ / (g_cdc_set_line_coding.dwDTERate << 2)) - 1);
    SPBRG  = (uint8_t)calc_SPBRG;
    SPBRGH = (uint8_t)(calc_SPBRG >> 8);
}

void cdc_data_out(void)
{
    m_serial_pkt_rcv = true;
}

void cdc_data_in(void)
{
    m_serial_pkt_sent = true;
}

void cdc_notification(void)
{
    #if defined(USE_DTR) || defined(USE_DCD)
    g_cdc_sent_last_notification = true;
    #endif
}

static void vcp_tasks(void)
{
    // If UART packet received, add to m_rx_buffer.
    if(PIR1bits.RCIF)
    {
        #ifdef USE_RTS
        if(m_rx_index == RX_BUFFER_SIZE - 5) // If buffer is almost full
        {
            m_rx_buffer_full = true;
            RTS = RTS_ACTIVE ^ 1;
        }
        #endif
        if(m_rx_index < RX_BUFFER_SIZE) m_rx_buffer[m_rx_index++] = UART_Read(); // If m_rx_index is not out of bounds, read the Rx data and put in buffer.
    }                                                                            // If this is false (buffer is full) data will be lost.

    // If there is data in m_rx_buffer and USB is not busy, send it.
    if(m_serial_pkt_sent && m_rx_index)
    {
        m_serial_pkt_sent = false;
        usb_ram_copy(m_rx_buffer, g_cdc_dat_ep_in, m_rx_index);
        cdc_arm_data_ep_in(m_rx_index);
        m_rx_index = 0;
        #ifdef USE_RTS
        if(g_cdc_has_set_rts && (RTS != RTS_ACTIVE)) RTS = RTS_ACTIVE;
        m_rx_buffer_full = false;
        #endif
    }

    // If serial data was received on the CDC endpoint, and UART currently has no data to send.
    if(m_serial_pkt_rcv && (m_tx_to_cpy == 0))
    {
        m_serial_pkt_rcv = false;                // Reset m_serial_pkt_rcv.
        m_tx_to_cpy = g_cdc_num_data_out;        // Number of bytes to grab from EP.
        m_tx_index = 0;                          // Reset m_tx_index.
        usb_ram_copy(g_cdc_dat_ep_out, m_tx_buffer, m_tx_to_cpy); // Copy from endpoint to m_tx_buffer.
        cdc_arm_data_ep_out();
    }

    // If there is data in m_tx_buffer, send one byte over UART.
    if(m_tx_to_cpy)
    {
        #if defined(USE_RTS) && !defined(USE_DTR)
        if(CTS == CTS_ACTIVE) // Block if CTS is not active.
        {
            UART_Write(m_tx_buffer[m_tx_index]);
            m_tx_index++;
            m_tx_to_cpy--;
        }
        #elif !defined(USE_RTS) && defined(USE_DTR)
        if(DSR == DSR_ACTIVE) // Block if DSR is not active.
        {
            UART_Write(m_tx_buffer[m_tx_index]);
            m_tx_index++;
            m_tx_to_cpy--;
        }
        #elif defined(USE_RTS) && defined(USE_DTR)
        if(CTS == CTS_ACTIVE && DSR == DSR_ACTIVE) // Block if either CTS or DSR is not active.
        {
            UART_Write(m_tx_buffer[m_tx_index]);
            m_tx_index++;
            m_tx_to_cpy--;
        }
        #else
        UART_Write(m_tx_buffer[m_tx_index]);
        m_tx_index++;
        m_tx_to_cpy--;
        #endif
    }

    #ifdef USE_DTR
    if((DSR ^ DSR_ACTIVE) == g_cdc_serial_state.bTxCarrier) // Detect change on DSR.
    {
        g_cdc_serial_state.bTxCarrier ^= 1;
        g_cdc_send_notification = true;
    }
    #endif
    #ifdef USE_DCD
    if((DCD ^ DCD_ACTIVE) == g_cdc_serial_state.bRxCarrier) // Detect change on DCD.
    {
        g_cdc_serial_state.bRxCarrier ^= 1;
        g_cdc_send_notification = true;
    }
    #endif
    #if defined(USE_DTR) || defined(USE_DCD)
    cdc_notification_tasks();
    #endif
}