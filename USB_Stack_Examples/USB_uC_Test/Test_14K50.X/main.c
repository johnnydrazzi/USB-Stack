/*
 * File:   main.c
 * Author: e5105180
 *
 * Created on 1 March 2018, 5:31 PM
 */

#include <xc.h>

#define _XTAL_FREQ 12000000

#define LED LATCbits.LATC1
#define LED_TRIS TRISCbits.TRISC1

#define BUTTON PORTCbits.RC0

void main(void) {
    ANSEL = 0;
    LED = 0;
    LED_TRIS = 0;
    while(1){
        if(BUTTON){
            LED = 1;
            __delay_ms(250);
            LED = 0;
            __delay_ms(250);
        }
        else{
            LED = 1;
            __delay_ms(500);
            LED = 0;
            __delay_ms(500);
        }
    }
    return;
}
