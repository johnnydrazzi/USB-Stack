/**
 * @file config.h
 * @brief PIC Microcontroller Settings.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * PIC18F1XK50 Configuration File.
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

#define _XTAL_FREQ 48000000
#define PLL_STARTUP_DELAY() __delay_ms(3)

#define BUTTON      PORTCbits.RC0
#define LED         LATCbits.LATC1

#define BUTTON_TRIS TRISCbits.TRISC0
#define LED_TRIS    TRISCbits.TRISC1

#endif /* CONFIG_H */