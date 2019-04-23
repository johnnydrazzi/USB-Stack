/**
 * @file config.h
 * @brief PIC Microcontroller Settings.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * PIC18FXXJ53 Configuration File.
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

#define MHz_4  1
#define MHz_8  2
#define MHz_12 3
#define MHz_16 4
#define MHz_20 5
#define MHz_24 6
#define MHz_40 10
#define MHz_48 12


#define XTAL_USED MHz_16

#define _XTAL_FREQ 48000000
#define PLL_STARTUP_DELAY() __delay_ms(3)

#define BUTTON      PORTBbits.RB6
#define LED         LATBbits.LB7

#define BUTTON_TRIS TRISBbits.TRISB6
#define LED_TRIS    TRISBbits.TRISB7

#endif /* CONFIG_H */