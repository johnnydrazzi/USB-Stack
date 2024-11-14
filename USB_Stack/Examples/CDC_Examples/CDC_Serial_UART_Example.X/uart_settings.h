/*
 * File:   uart_settings.h
 * Author: John Izzard
 *
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

#ifndef UART_SETTINGS_H
#define	UART_SETTINGS_H

#define UART1_BAUD 9600
#define UART2_BAUD 9600

#if defined(_16F1454) || defined(_16F1455)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISCbits.TRISC4
#define UART1_RX_TRIS TRISCbits.TRISC5
#elif defined(_16F1459)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISBbits.TRISB7
#define UART1_RX_TRIS TRISBbits.TRISB5
#elif defined(_16F87) || defined(_16F88)
#define BAUD_8BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISBbits.TRISB5
#define UART1_RX_TRIS TRISBbits.TRISB2
#elif defined(_16F18313)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISAbits.TRISA4
#define UART1_RX_TRIS TRISAbits.TRISA5
#define HAS_PPS1
#define PPS_LOCKED   0
#define PPS_UNLOCKED 1
#define PPS_RX1_REG RXPPS
#define PPS_RX1_VAL 5      // RX is on RA5
#define PPS_TX1_REG RA4PPS // TX is on RA5
#define PPS_TX1_VAL 0x14
#elif defined(_18F4520)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#elif defined(_18F25K22)
#define BAUD_16BITS
#define NUM_UARTS 2
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#define UART2_TX_TRIS TRISBbits.TRISB6
#define UART2_RX_TRIS TRISBbits.TRISB7
#elif defined(_18F45K22)
#define BAUD_16BITS
#define NUM_UARTS 2
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#define UART2_TX_TRIS TRISDbits.TRISD6
#define UART2_RX_TRIS TRISDbits.TRISD7
#elif defined(_18F47J53_FAMILY_) || defined(_18F26J53) || defined(_18F46J53) || defined(_18F46J50_FAMILY_)
#define BAUD_16BITS
#define NUM_UARTS 2
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#define UART2_TX_TRIS TRISBbits.TRISB6
#define UART2_RX_TRIS TRISBbits.TRISB7
#define HAS_PPS2
#define PPSLOCK PPSCONbits.IOLOCK
#define PPS_LOCKED   1
#define PPS_UNLOCKED 0
#define PPS_RX2_REG RPINR16
#define PPS_RX2_VAL 10    // RX2 is on RB7 (RP10)
#define PPS_TX2_REG RPOR9 // TX2 is on RB6 (RP9)
#define PPS_TX2_VAL 6
#elif defined(_18F13K50) || defined(_18F14K50)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISBbits.TRISB7
#define UART1_RX_TRIS TRISBbits.TRISB5
#elif defined(_18F24K50) || defined(_18F25K50) || defined(_18F45K50) || \
      defined(_18F4520_FAMILY_) || defined(_18F4450_FAMILY_) || defined(_18F4550_FAMILY_)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#elif defined(__18F26K80) || defined(__18F46K80)
#define BAUD_16BITS
#define NUM_UARTS 2
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#define UART2_TX_TRIS TRISBbits.TRISB6
#define UART2_RX_TRIS TRISBbits.TRISB7
#else
#error UART - DEVICE NOT YET SUPPORTED
#endif

#ifndef BAUDCON
#define BAUDCON BAUDCON1
#endif

#ifndef BAUDCON
#define BAUDCONbits BAUDCON1bits
#endif

#ifndef SPBRGH
#define SPBRGH SPBRGH1
#endif

#endif	/* UART_SETTINGS_H */

