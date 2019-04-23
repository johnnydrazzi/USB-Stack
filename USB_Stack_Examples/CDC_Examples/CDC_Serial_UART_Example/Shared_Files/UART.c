/*
 * File:   UART.c
 * Author: John Izzard
 *
 * Year:   2016
 */

#include <xc.h>
#include "UART.h"

#ifdef PIC32MX795F512L
#define UART1_CALC (((((float)CLOCK_FREQ/UART1_BAUD)/4)-1)+0.5)
/* ************************* UART1 ************************* */
void UART1_Init(void) {
    uint16_t x;
    double PBCLK = _XTAL_FREQ;

    TRISFbits.TRISF8 = 1;
    TRISFbits.TRISF8 = 1;

    U1MODEbits.BRGH = 1;// High rate

    U1BRG = (uint16_t)UART1_CALC;

    U1STAbits.UTXEN = 1; // Enable transmitter
    U1STAbits.URXEN = 1; // Enable Receiver

    U1MODEbits.ON = 1; // Enable serial port
}
uint8_t UART1_Data_Ready(){
    if(IFS0bits.U1RXIF){IFS0bits.U1RXIF=0;return 1;}
    else{return 0;}
}
uint8_t UART1_Tx_Idle(){
    if(U1STAbits.TRMT){return 1;}
    else{return 0;}
}
uint8_t UART1_Read(){
    uint8_t temp;
    if(U1STAbits.OERR){
                        temp = U1RXREG;
                        U1STAbits.OERR = 0;

                        return 0;
                       }
    if(U1STAbits.FERR||U1STAbits.PERR){
                        temp = U1RXREG;

                        return 0;
                       }

    return U1RXREG;
}
void UART1_Read_Text(uint8_t *Output, uint8_t *Delimiter, uint8_t Attempts){
    uint8_t i;
    uint8_t x = 0;
    uint16_t del_address = (uint16_t)Delimiter;
    uint16_t out_address = (uint16_t)Output;

    while(1){                       // Clears all elements in Output array
        if(*Output==0){break;}      // until it sees the first Null character
        else{*Output = 0; Output++;}
    }
    Output = (uint8_t *)out_address;   // Set pointer back to start of Output array

    if(Attempts != 255){
        for(i=0;i<Attempts;i++,Output++){

            while(!UART1_Data_Ready()){}
            *Output = UART1_Read();

            if(*Output==*Delimiter){Delimiter++; x++; if(*Delimiter==0){break;}}
            else{Delimiter=(uint8_t *)del_address;}
        }
    }
    else{
        while(1){

            while(!UART1_Data_Ready()){}
            *Output = UART1_Read();

            if(*Output==*Delimiter){Delimiter++; x++; if(*Delimiter==0){break;}}
            else{Delimiter=(uint8_t *)del_address;}
        }
    }

    for(i=x;i!=0;i--,Output--){*Output=0;} // Remove delimiter
}
void UART1_Write(uint8_t data_){
    while(!UART1_Tx_Idle()){};
    U1TXREG = data_;
}
void UART1_Write_Text(uint8_t *UART_text){
    while(*UART_text){
        UART1_Write(*UART_text++);
    }
}
#endif

#ifdef __XC8
#if NUM_UARTS >= 1
/* ************************* UART1 ************************* */
#ifdef BAUD_8BITS
#define UART1_CALC (((((float)CLOCK_FREQ/UART1_BAUD)/16)-1)+0.5)
#else
#define UART1_CALC (((((float)CLOCK_FREQ/UART1_BAUD)/4)-1)+0.5)
#define UART1_CALC_H (UART1_CALC/256)
#endif /* BAUD_8BITS */
void UART1_Init(void) {
#ifdef HAS_PPS1
    PPSLOCK = PPS_UNLOCKED;
    PPS_RX1_REG = PPS_RX1_VAL;
    PPS_TX1_REG = PPS_TX1_VAL;
    PPSLOCK = PPS_LOCKED;
#endif
    UART1_TX_TRIS = 1;
    UART1_RX_TRIS = 1;

    TXSTAbits.BRGH = 1; // High rate
    SPBRG = (uint8_t)UART1_CALC;
    
#ifdef BAUD_16BITS
    BAUDCONbits.BRG16 = 1; // 16-bit baud rate generator
    SPBRGH = (uint8_t)UART1_CALC_H;
#endif

    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1; // Enable transmitter
    RCSTAbits.CREN = 1; // Enable Receiver
    RCSTAbits.SPEN = 1; // Enable serial port
}
uint8_t UART1_Data_Ready(void){
    if(PIR1bits.RCIF){return 1;}
    else{return 0;}
}
uint8_t UART1_Tx_Idle(void){
    if(TXSTAbits.TRMT){return 1;}
    else{return 0;}
}
uint8_t UART1_Read(void){
    volatile uint8_t temp;
    if(RCSTAbits.OERR){
        temp = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        return 0;
    }
    if(RCSTAbits.FERR){
        while(RCSTAbits.FERR){temp = RCREG;}
        return 0;
    }

    return RCREG;
}
void UART1_Read_Text(uint8_t *Output, uint8_t *Delimiter, uint8_t Attempts){
    uint8_t i;
    uint8_t x = 0;
    uint16_t del_address = (uint16_t)Delimiter;
    uint16_t out_address = (uint16_t)Output;

    while(1){                       // Clears all elements in Output array
        if(*Output==0){break;}      // until it sees the first Null character
        else{*Output = 0; Output++;}
    }
    Output = (uint8_t *)out_address;   // Set pointer back to start of Output array

    if(Attempts != 255){
        for(i=0;i<Attempts;i++,Output++){

            while(!UART1_Data_Ready()){}
            *Output = UART1_Read();

            if(*Output==*Delimiter){Delimiter++; x++; if(*Delimiter==0){break;}}
            else{Delimiter=(uint8_t *)del_address;}
        }
    }
    else{
        while(1){

            while(!UART1_Data_Ready()){}
            *Output = UART1_Read();

            if(*Output==*Delimiter){Delimiter++; x++; if(*Delimiter==0){break;}}
            else{Delimiter=(uint8_t *)del_address;}
        }
    }

    for(i=x;i!=0;i--,Output--){*Output=0;} // Remove delimiter
}
void UART1_Write(uint8_t data_){
    while(!UART1_Tx_Idle()){};
    TXREG = data_;
}
void UART1_Write_Const(const uint8_t data_){
    while(!UART1_Tx_Idle()){};
    TXREG = data_;
}
void UART1_Write_Text(uint8_t *UART_text){
    while(*UART_text){
        UART1_Write(*UART_text++);
    }
}
void UART1_Write_Const_Text(const uint8_t *UART_text){
    while(*UART_text){
        UART1_Write_Const(*UART_text++);
    }
}
#endif
#if NUM_UARTS >= 2
/* ************************* UART2 ************************* */
#ifdef BAUD_8BITS
#define UART2_CALC (((((float)CLOCK_FREQ/UART2_BAUD)/16)-1)+0.5)
#else
#define UART2_CALC (((((float)CLOCK_FREQ/UART2_BAUD)/4)-1)+0.5)
#define UART2_CALC_H (UART2_CALC/256)
#endif /* BAUD_8BITS */
void UART2_Init(void) {
#ifdef HAS_PPS2
    PPSLOCK = PPS_UNLOCKED;
    PPS_RX2_REG = PPS_RX2_VAL;
    PPS_TX2_REG = PPS_TX2_VAL;
    PPSLOCK = PPS_LOCKED;
#endif
    UART2_TX_TRIS = 1;
    UART2_RX_TRIS = 1;

    TXSTA2bits.BRGH = 1; // High rate
    SPBRG2 = (uint8_t)UART2_CALC;
    
#ifdef BAUD_16BITS
    BAUDCON2bits.BRG16 = 1; // 16-bit baud rate generator
    SPBRGH2 = (uint8_t)UART2_CALC_H;
#endif

    TXSTA2bits.SYNC = 0;
    TXSTA2bits.TXEN = 1; // Enable transmitter
    RCSTA2bits.CREN = 1; // Enable Receiver
    RCSTA2bits.SPEN = 1; // Enable serial port
}
uint8_t UART2_Data_Ready(void){
    if(PIR3bits.RC2IF){return 1;}
    else{return 0;}
}
uint8_t UART2_Tx_Idle(void){
    if(TXSTA2bits.TRMT){return 1;}
    else{return 0;}
}
uint8_t UART2_Read(){
    volatile uint8_t temp;
    if(RCSTA2bits.OERR){
        temp = RCREG2;
        RCSTA2bits.CREN = 0;
        RCSTA2bits.CREN = 1;
        return 0;
    }
    if(RCSTA2bits.FERR){
        while(RCSTA2bits.FERR){temp = RCREG1;}
        return 0;
    }

    return RCREG2;
}
void UART2_Read_Text(uint8_t *Output, uint8_t *Delimiter, uint8_t Attempts){
    uint8_t i;
    uint8_t x = 0;
    uint16_t del_address = (uint16_t)Delimiter;
    uint16_t out_address = (uint16_t)Output;

    while(1){                       // Clears all elements in Output array
        if(*Output==0){break;}      // until it sees the first Null character
        else{*Output = 0; Output++;}
    }
    Output = (uint8_t *)out_address;             // Set pointer back to start of Output array

    if(Attempts != 255){
        for(i=0;i<Attempts;i++,Output++){

            while(!UART2_Data_Ready()){}
            *Output = UART2_Read();

            if(*Output==*Delimiter){Delimiter++; x++; if(*Delimiter==0){break;}}
            else{Delimiter=(uint8_t *)del_address;}
        }
    }
    else{
        while(1){

            while(!UART2_Data_Ready()){}
            *Output = UART2_Read();

            if(*Output==*Delimiter){Delimiter++; x++; if(*Delimiter==0){break;}}
            else{Delimiter=(uint8_t *)del_address;}
        }
    }

    for(i=x;i!=0;i--,Output--){*Output=0;} // Remove delimiter
}
void UART2_Write(uint8_t data_){
    while(!UART2_Tx_Idle()){};
    TXREG2 = data_;
}
void UART2_Write_Const(const uint8_t data_){
    while(!UART2_Tx_Idle()){};
    TXREG2 = data_;
}
void UART2_Write_Text(uint8_t *UART_text){
    while(*UART_text){
        UART2_Write(*UART_text++);
    }
}
void UART2_Write_Const_Text(const uint8_t *UART_text){
    while(*UART_text){
        UART2_Write_Const(*UART_text++);
    }
}
#endif /* NUM_UARTS >= 2 */
#endif /* __XC8 */