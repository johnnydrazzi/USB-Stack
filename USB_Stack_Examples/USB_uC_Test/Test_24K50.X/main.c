/*
 * File:   main.c
 * Author: e5105180
 *
 * Created on 1 March 2018, 5:31 PM
 */

#include <xc.h>

#define NO_XTAL 0
#define MHz_12  1
#define MHz_16  2

#define NORMAL 0
#define DEV_BOARD 1

#define XTAL_USED NO_XTAL
#define PIN_VERSION NORMAL

#if XTAL_USED == MHz_12
#define _XTAL_FREQ 12000000
#else
#define _XTAL_FREQ 16000000
#endif

#if XTAL_USED == NO_XTAL && PIN_VERSION == NORMAL
#define BUTTON      PORTAbits.RA6
#define LED         LATAbits.LA7
#define BUTTON_TRIS TRISAbits.TRISA6
#define LED_TRIS    TRISAbits.TRISA7
#else
#define BUTTON      PORTBbits.RB6
#define LED         LATBbits.LB7
#define BUTTON_TRIS TRISBbits.TRISB6
#define LED_TRIS    TRISBbits.TRISB7
#endif

void main(void) {
#if XTAL_USED == NO_XTAL
    OSCCONbits.IRCF = 7;
    ANSELA = 0;
#endif
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
