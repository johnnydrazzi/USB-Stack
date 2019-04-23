/**
 * @file config.h
 * @brief PIC Microcontroller Settings.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * PIC16F145X Configuration File.
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
#ifndef CONFIG_H
#define CONFIG_H

#define NO_XTAL 0
#define MHz_12  1
#define MHz_16  2

#define XTAL_USED NO_XTAL

#define _XTAL_FREQ 48000000
#define PLL_STARTUP_DELAY() __delay_ms(3)

//#define XPRESS

#ifdef XPRESS
#define BUTTON      !PORTAbits.RA5
#define LED         LATCbits.LATC3

#define BUTTON_TRIS TRISAbits.TRISA5
#define LED_TRIS    TRISCbits.TRISC3
#else
#define BUTTON      PORTCbits.RC4
#define LED         LATCbits.LATC5

#define BUTTON_TRIS TRISCbits.TRISC4
#define LED_TRIS    TRISCbits.TRISC5
#endif

#endif /* CONFIG_H */