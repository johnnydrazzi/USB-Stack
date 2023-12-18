/**
 * @file usb_cdc_config.h
 * @brief <i>Communications Device Class</i> core settings.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * USB uC - CDC Library.
 * Copyright (C) 2017-2020  John Izzard
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

#ifndef USB_CDC_CONFIG_H
#define USB_CDC_CONFIG_H

#include "usb_cdc.h"
#include <xc.h>

/* ************************************************************************** */
/* ************************* SET LINE CODING SETTINGS *********************** */
/* ************************************************************************** */

#define STARTING_BAUD      9600
#define STARTING_STOP_BITS STOP_BIT_1
#define STARTING_PARITY    PARITY_NONE
#define STARTING_DATA_BITS 8 // 5, 6, 7, 8, or 16.

/* ************************************************************************** */


/* ************************************************************************** */
/* **************************** REQUESTS USED ******************************* */
/* ************************************************************************** */

//#define USE_SET_COMM_FEATURE   // Not yet implemented.
//#define USE_GET_COMM_FEATURE   // Not yet implemented.
//#define USE_CLEAR_COMM_FEATURE // Not yet implemented.
#define USE_SET_LINE_CODING
#define USE_GET_LINE_CODING
#define USE_SET_CONTROL_LINE_STATE
//#define USE_SEND_BREAK         // Not yet implemented.

/* ************************************************************************** */


/* ************************************************************************** */
/* ************************ HW FLOW CONTROL SETTINGS ************************ */
/* ************************************************************************** */

//#define USE_DCD
#define DCD_ACTIVE 0
#define DCD        PORTBbits.RB0

//#define USE_DTR
#define DTR_ACTIVE 0
#define DSR_ACTIVE 0
#define DTR        LATBbits.LATB1
#define DSR        PORTBbits.RB2
#define DTR_TRIS   TRISBbits.TRISB1

//#define USE_RTS
#define RTS_ACTIVE 0
#define CTS_ACTIVE 0
#define RTS        LATBbits.LATB3
#define CTS        PORTBbits.RB4
#define RTS_TRIS   TRISBbits.TRISB3

/* ************************************************************************** */


/* ************************************************************************** */
/* ***************************** CDC INTERFACE ****************************** */
/* ************************************************************************** */

// Communication Class Interface Number
#define CDC_COM_INT 0

/* ************************************************************************** */


/* ************************************************************************** */
/* ***************************** CDC ENDPOINTS ****************************** */
/* ************************************************************************** */

// CDC Endpoint HAL
#define CDC_COM_EP EP1
#define CDC_DAT_EP EP2
#define CDC_COM_EP_SIZE EP1_SIZE
#define CDC_DAT_EP_SIZE EP2_SIZE

/* ************************************************************************** */


/* ************************************************************************** */
/* *************************** CDC BD LOCATIONS ***************************** */
/* ************************************************************************** */

#define CDC_COM_BD_IN  BD1_IN
#define CDC_DAT_BD_OUT BD2_OUT
#define CDC_DAT_BD_IN  BD2_IN

/* ************************************************************************** */


/* ************************************************************************** */
/* ******************************* UEPn HAL ********************************* */
/* ************************************************************************** */

#define CDC_COM_UEPbits UEP1bits
#define CDC_DAT_UEPbits UEP2bits

/* ************************************************************************** */

#endif