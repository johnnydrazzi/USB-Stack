/*
 * File:   uart.c
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

#define UART_SPBRG_CALC(freq, baud, div) (((float)(((freq / baud) / div) - 1)) + 0.5)
#define UART_SPBRG_CALC_8(freq, baud) UART_SPBRG_CALC(freq, baud, 16)
#define UART_SPBRG_CALC_16(freq, baud) UART_SPBRG_CALC(freq, baud, 4)

#ifdef BAUD_8BITS
#define UART1_SPBRG_DEFAULT UART_SPBRG_CALC_8(_XTAL_FREQ, UART1_BAUD)
#define UART2_SPBRG_DEFAULT UART_SPBRG_CALC_8(_XTAL_FREQ, UART2_BAUD)
#else
#define UART1_SPBRG_DEFAULT UART_SPBRG_CALC_16(_XTAL_FREQ, UART1_BAUD)
#define UART2_SPBRG_DEFAULT UART_SPBRG_CALC_16(_XTAL_FREQ, UART2_BAUD)
#define UART1_SPBRGH_DEFAULT (UART1_SPBRG_DEFAULT / 256)
#define UART2_SPBRGH_DEFAULT (UART2_SPBRG_DEFAULT / 256)
#endif /* BAUD_8BITS */

#include <xc.h>
#include "uart.h"

/* STATIC PROTOTYPES */
static void    init1(void);
static void    set_baud1(uint16_t baud_calc);
static bool    data_ready1(void);
static bool    tx_idle1(void);
static uint8_t read1(void);
static void    write1(uint8_t byte);

static void    init2(void);
static void    set_baud2(uint16_t baud_calc);
static bool    data_ready2(void);
static bool    tx_idle2(void);
static uint8_t read2(void);
static void    write2(uint8_t byte);

static void    read_string(uint8_t* output, uint8_t* delimiter, uint8_t attempts, bool (*data_ready)(void), uint8_t (*read)(void));
static void    write_string(uint8_t* string, bool (*tx_idle)(void), void (*write)(uint8_t));


/* GLOBAL FUNCTIONS */
void uart__init(uint8_t uart)
{
    switch(uart)
    {
        case 0:
            init1();
            break;
        #if NUM_UARTS >= 2
        case 1:
            init2();
            break;
        #endif
        default:
            break; // This uart doesn't exist.
    }
}

void uart__set_baud(uint8_t uart, uint16_t baud)
{
    #if BAUD_8BITS
    uint16_t baud_calc = (uint16_t)UART_SPBRG_CALC_8(_XTAL_FREQ, baud);
    #else
    uint16_t baud_calc = (uint16_t)UART_SPBRG_CALC_16(_XTAL_FREQ, baud);
    #endif

    switch(uart)
    {
        case 0:
            set_baud1(baud_calc);
            break;
        #if NUM_UARTS >= 2
        case 1:
            set_baud2(baud_calc);
            break;
        #endif
        default:
            break; // This uart doesn't exist.
    }
}

bool uart__data_ready(uint8_t uart)
{
    switch(uart)
    {
        case 0:
            return data_ready1();
        #if NUM_UARTS >= 2
        case 1:
            return data_ready2();
        #endif
        default:
            return true; // This uart doesn't exist, return true.
    }
}

bool uart__tx_idle(uint8_t uart)
{
    switch(uart)
    {
        case 0:
            return tx_idle1();
        #if NUM_UARTS >= 2
        case 1:
            return tx_idle2();
        #endif
        default:
            return true; // This uart doesn't exist, return true.
    }
}

uint8_t uart__read(uint8_t uart)
{
    switch(uart)
    {
        case 0:
            return read1();
        #if NUM_UARTS >= 2
        case 1:
            return read2();
        #endif
        default:
            return 0; // This uart doesn't exist, returning something.
    }
}

void uart__read_string(uint8_t uart, uint8_t* output, uint8_t* delimiter, uint8_t attempts)
{
    switch(uart)
    {
        case 0:
            read_string(output, delimiter, attempts, data_ready1, read1);
            break;
        #if NUM_UARTS >= 2
        case 1:
            read_string(output, delimiter, attempts, data_ready2, read2);
            break;
        #endif
        default:
            break; // This uart doesn't exist.
    }
}

void uart__write(uint8_t uart, uint8_t byte)
{
    switch(uart)
    {
        case 0:
            write1(byte);
            break;
        #if NUM_UARTS >= 2
        case 1:
            write2(byte);
            break;
        #endif
        default:
            break; // This uart doesn't exist.
    }
}

void uart__write_string(uint8_t uart, uint8_t* string)
{
    switch(uart)
    {
        case 0:
            write_string(string, tx_idle1, write1);
            break;
        #if NUM_UARTS >= 2
        case 1:
            write_string(string, tx_idle2, write2);
            break;
        #endif
        default:
            break;  // This uart doesn't exist.
    }
}

/* STATIC FUNCTIONS */
static void init1(void)
{
    #ifdef HAS_PPS1
    PPSLOCK = PPS_UNLOCKED;
    PPS_RX1_REG = PPS_RX1_VAL;
    PPS_TX1_REG = PPS_TX1_VAL;
    PPSLOCK = PPS_LOCKED;
    #endif /* HAS_PPS1 */
    UART1_TX_TRIS = 1;
    UART1_RX_TRIS = 1;

    TXSTAbits.BRGH = 1; // High rate.
    SPBRG = (uint8_t)UART1_SPBRG_DEFAULT;
    
    #ifdef BAUD_16BITS
    BAUDCONbits.BRG16 = 1; // 16-bit baud rate generator.
    SPBRGH = (uint8_t)UART1_SPBRGH_DEFAULT;
    #endif /* BAUD_16BITS */

    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1; // Enable transmitter.
    RCSTAbits.CREN = 1; // Enable Receiver.
    RCSTAbits.SPEN = 1; // Enable serial port.
}

static void set_baud1(uint16_t baud_calc)
{
    SPBRG = (uint8_t)baud_calc;
    #ifdef BAUD_16BITS
    SPBRGH = (uint8_t)(baud_calc >> 8);
    #endif
}

static bool data_ready1(void)
{
    return PIR1bits.RCIF;
}

static bool tx_idle1(void)
{
    return TXSTAbits.TRMT;
}

static uint8_t read1(void)
{
    volatile uint8_t temp;
    if(RCSTAbits.OERR)
    {
        // Clear buffer overrun error by reading RCREG.
        temp = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        return 0;
    }
    if(RCSTAbits.FERR)
    {
        // Clear framing error by reading RCREG until FERR is cleared.
        while(RCSTAbits.FERR) temp = RCREG;
        return 0;
    }

    return RCREG;
}

static void write1(uint8_t byte)
{
    TXREG = byte;
}

#if NUM_UARTS >= 2
static void init2(void)
{
    #ifdef HAS_PPS2
    PPSLOCK = PPS_UNLOCKED;
    PPS_RX2_REG = PPS_RX2_VAL;
    PPS_TX2_REG = PPS_TX2_VAL;
    PPSLOCK = PPS_LOCKED;
    #endif /* HAS_PPS2 */
    UART2_TX_TRIS = 1;
    UART2_RX_TRIS = 1;

    TXSTA2bits.BRGH = 1; // High rate.
    SPBRG2 = (uint8_t)UART2_SPBRG_DEFAULT;
    
    #ifdef BAUD_16BITS
    BAUDCON2bits.BRG16 = 1; // 16-bit baud rate generator.
    SPBRGH2 = (uint8_t)UART2_SPBRGH_DEFAULT;
    #endif /* BAUD_16BITS */

    TXSTA2bits.SYNC = 0;
    TXSTA2bits.TXEN = 1; // Enable transmitter.
    RCSTA2bits.CREN = 1; // Enable Receiver.
    RCSTA2bits.SPEN = 1; // Enable serial port.
}

static void set_baud2(uint16_t baud_calc)
{
    SPBRG2 = (uint8_t)baud_calc;
    #ifdef BAUD_16BITS
    SPBRGH2 = (uint8_t)(baud_calc >> 8);
    #endif
}

static bool data_ready2(void)
{
    return PIR3bits.RC2IF;
}

static bool tx_idle2(void)
{
    return TXSTA2bits.TRMT;
}

static uint8_t read2(void)
{
    volatile uint8_t temp;
    if(RCSTA2bits.OERR)
    {
        // Clear buffer overrun error by reading RCREG.
        temp = RCREG2;
        RCSTA2bits.CREN = 0;
        RCSTA2bits.CREN = 1;
        return 0;
    }
    if(RCSTA2bits.FERR)
    {
        // Clear framing error by reading RCREG until FERR is cleared.
        while(RCSTA2bits.FERR) temp = RCREG2;
        return 0;
    }

    return RCREG2;
}

static void write2(uint8_t byte)
{
    TXREG2 = byte;
}
#endif /* NUM_UARTS >= 2 */

static void read_string(uint8_t* output, uint8_t* delimiter, uint8_t attempts, bool (*data_ready)(void), uint8_t (*read)(void))
{
    uint8_t  del_i = 0;
    uint8_t* del_start = delimiter;
    uint8_t* out_start = output;
    bool     del_found = false;
    
    // Clears all elements in output array.
    while(*output) *(output++) = 0;
    
    // Set pointer back to start of output array.
    output = out_start;

    for(uint8_t i = 1; i <= attempts; i++, output++)
    {
        while(!data_ready()){}
        *output = read();

        if(*output == *delimiter)
        {
            delimiter++;
            del_i++;
            if(*delimiter == 0)
            {
                del_found = true;
                break;
            }
        }
        else delimiter = del_start;
    }

    if(del_found)
    {
        while(del_i--) *(output--) = 0; // Remove delimiter.
    }
    else out_start[0] = 0; // Return an empty string.
}

static void write_string(uint8_t* string, bool (*tx_idle)(void), void (*write)(uint8_t))
{
    while(*string)
    {
        while(!tx_idle()){}
        write(*string++);
    }
}