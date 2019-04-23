/*
 * File:   UART.h
 * Author: John Izzard
 *
 * Year:   2016
 */

#ifndef UART_H
#define UART_H

#define UART_Init             UART1_Init
#define UART_Data_Ready       UART1_Data_Ready
#define UART_Tx_Idle          UART1_Tx_Idle
#define UART_Read             UART1_Read
#define UART_Read_Text        UART1_Read_Text
#define UART_Write            UART1_Write
#define UART_Write_Const      UART1_Write_Const
#define UART_Write_Text       UART1_Write_Text
#define UART_Write_Const_Text UART1_Write_Const_Text

/* ********* SETTINGS ********** */
#define CLOCK_FREQ 48000000
#define UART1_BAUD 9600
#define UART2_BAUD 9600

#if defined(_PIC14E)
#define BAUD_16BITS
#define NUM_UARTS 1
#if defined(__16F1459)
#define UART1_TX_TRIS TRISBbits.TRISB7
#define UART1_RX_TRIS TRISBbits.TRISB5
#else
#define UART1_TX_TRIS TRISCbits.TRISC4
#define UART1_RX_TRIS TRISCbits.TRISC5
#endif
#elif defined(__16F87)||defined(__16F88)
#define BAUD_8BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISBbits.TRISB5
#define UART1_RX_TRIS TRISBbits.TRISB2
#elif defined(__16F18313)
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
#elif defined(__18F4520)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#elif defined(__18F25K22)
#define BAUD_16BITS
#define NUM_UARTS 2
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#define UART2_TX_TRIS TRISBbits.TRISB6
#define UART2_RX_TRIS TRISBbits.TRISB7
#elif defined(__18F45K22)
#define BAUD_16BITS
#define NUM_UARTS 2
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#define UART2_TX_TRIS TRISDbits.TRISD6
#define UART2_RX_TRIS TRISDbits.TRISD7
#elif defined(__18F26J53)||defined(__18F46J53)||defined(__18F27J53)||defined(__18F47J53)
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
#elif defined(__18F13K50)||defined(__18F14K50)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISBbits.TRISB7
#define UART1_RX_TRIS TRISBbits.TRISB5
#elif defined(__18F24K50)||defined(__18F25K50)||defined(__18F45K50)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISCbits.TRISC6
#define UART1_RX_TRIS TRISCbits.TRISC7
#elif defined(__18F13K50)||defined(__18F14K50)
#define BAUD_16BITS
#define NUM_UARTS 1
#define UART1_TX_TRIS TRISBbits.TRISB7
#define UART1_RX_TRIS TRISBbits.TRISB5
#else
#error UART - DEVICE NOT YET SUPPORTED
#endif

#include <stdint.h>

#if NUM_UARTS >= 1
/* *********** UART1 ************ */
void UART1_Init(void);
char UART1_Data_Ready(void);
char UART1_Tx_Idle(void);
char UART1_Read(void);
void UART1_Read_Text(uint8_t *Output, uint8_t *Delimiter, uint8_t Attempts);
void UART1_Write(uint8_t data_);
void UART1_Write_Const(const uint8_t data_);
void UART1_Write_Text(uint8_t *UART_text);
void UART1_Write_Const_Text(const uint8_t *UART_text);
#endif

#if NUM_UARTS >= 2
/* *********** UART2 ************ */
void UART2_Init(void);
char UART2_Data_Ready(void);
char UART2_Tx_Idle(void);
char UART2_Read(void);
void UART2_Read_Text(uint8_t *Output, uint8_t *Delimiter, uint8_t Attempts);
void UART2_Write(uint8_t data_);
void UART2_Write_Const(const uint8_t data_);
void UART2_Write_Text(uint8_t *UART_text);
void UART2_Write_Const_Text(const uint8_t *UART_text);
#endif

#endif /* UART_H */