/**
 * @file usb_config.h
 * @brief Contains core USB stack settings.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - USB Stack.
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

#ifndef USB_CONFIG_H
#define USB_CONFIG_H

/* ************************************************************************** */
/* **************************** SELECT EXAMPLE ****************************** */
/* ************************************************************************** */

#define CDC_EXAMPLE
//#define HID_CUSTOM_EXAMPLE
//#define HID_KEYBOARD_EXAMPLE
//#define HID_MOUSE_EXAMPLE
//#define MSD_INTERNAL_EXAMPLE
//#define MSD_SIMPLE_EXAMPLE

/* ************************************************************************** */


/* ************************************************************************** */
/* **************************** USB SETTINGS ******************************** */
/* ************************************************************************** */

#define BUS_POWERED  0
#define SELF_POWERED 1
#define POWERED_TYPE BUS_POWERED

#define LOW_SPEED  0
#define FULL_SPEED (1 << 2)
#define USB_SPEED  FULL_SPEED

#define SPEED_PULLUP_OFF 0
#define SPEED_PULLUP_ON  (1 << 4)
#define SPEED_PULLUP     SPEED_PULLUP_ON

#define REMOTE_WAKEUP_OFF 0
#define REMOTE_WAKEUP_ON  1
#define REMOTE_WAKEUP     REMOTE_WAKEUP_OFF

#define PINGPONG_DIS      0
#define PINGPONG_0_OUT    1
#define PINGPONG_ALL_EP   2
#define PINGPONG_1_15     3
#define PINGPONG_MODE     PINGPONG_0_OUT

#if defined(CDC_EXAMPLE)
#define NUM_CONFIGURATIONS 1
#define NUM_INTERFACES     2
#define NUM_ALT_INTERFACES 0
#define NUM_ENDPOINTS      3
#define EP0_SIZE           8
#define EP1_SIZE           10
#define EP2_SIZE           64

#elif defined(MSD_SIMPLE_EXAMPLE) || defined(MSD_INTERNAL_EXAMPLE) || defined(HID_CUSTOM_EXAMPLE)
#define NUM_CONFIGURATIONS 1
#define NUM_INTERFACES     1
#define NUM_ALT_INTERFACES 0
#define NUM_ENDPOINTS      2
#define EP0_SIZE           8
#define EP1_SIZE           64

#elif defined(HID_KEYBOARD_EXAMPLE) || defined(HID_MOUSE_EXAMPLE)
#define NUM_CONFIGURATIONS 1
#define NUM_INTERFACES     1
#define NUM_ALT_INTERFACES 0
#define NUM_ENDPOINTS      2
#define EP0_SIZE           8
#define EP1_SIZE           16
#else
// MAKE YOUR OWN
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* ************************* INTERRUPT SETTINGS ***************************** */
/* ************************************************************************** */

/*
 * INTERRUPT MASK OPTIONS:
 * _SOFIE   - Start Of Frame Interrupt (Optional, must define USE_SOF if used)
 * _STALLIE - Stall Interrupt (*not used)
 * _IDLEIE  - Idle Interrupt (Mandatory)
 * _TRNIE   - Transaction Complete Interrupt (Mandatory)
 * _ACTVIE  - Bus Activity Interrupt (Mandatory)
 * _UERIE   - USB Error Interrupt (Optional, must define USE_ERROR if used)
 * _URSTIE  - USB Reset Interrupt (Mandatory)
 */

#if defined(CDC_EXAMPLE)
#define INTERRUPTS_MASK (_IDLEIE | _TRNIE | _ACTVIE | _URSTIE)
#define ERROR_INTERRUPT_MASK 0

//#define USE_RESET
//#define USE_ERROR
//#define USE_IDLE
//#define USE_ACTIVITY
//#define USE_SOF
#define USE_OUT_CONTROL_FINISHED


#elif defined(MSD_SIMPLE_EXAMPLE) || defined(MSD_INTERNAL_EXAMPLE)
#define INTERRUPTS_MASK (_IDLEIE | _TRNIE | _ACTVIE | _URSTIE)
#define ERROR_INTERRUPT_MASK 0

//#define USE_RESET
//#define USE_ERROR
//#define USE_IDLE
//#define USE_ACTIVITY
//#define USE_SOF
//#define USE_OUT_CONTROL_FINISHED


#elif defined(HID_KEYBOARD_EXAMPLE) || defined(HID_MOUSE_EXAMPLE) || defined(HID_CUSTOM_EXAMPLE)
#define INTERRUPTS_MASK (_IDLEIE | _TRNIE | _ACTVIE | _URSTIE | _SOFIE)
#define ERROR_INTERRUPT_MASK 0

//#define USE_RESET
//#define USE_ERROR
//#define USE_IDLE
//#define USE_ACTIVITY
#define USE_SOF
//#define USE_OUT_CONTROL_FINISHED


#else
// MAKE YOUR OWN
#endif

/* ************************************************************************** */

#endif /* USB_CONFIG_H */