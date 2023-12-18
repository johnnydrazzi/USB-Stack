/**
 * @file usb_hid_config.h
 * @brief Contains HID user settings.
 * @author John Izzard
 * @date 24/02/2021
 * 
 * USB uC - USB Stack.
 * Copyright (C) 2017-2021  John Izzard
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
#ifndef USB_HID_CONFIG_H
#define USB_HID_CONFIG_H

#include "usb_config.h"

/* SETTINGS */
// Number of HID descriptors
#define HID_NUM_DESC 1

// Supported Class Requests - Uncomment to use
#define USE_SET_REPORT
#define USE_GET_IDLE
#define USE_SET_IDLE
//#define USE_GET_PROTOCOL // Not yet supported
//#define USE_SET_PROTOCOL // Not yet supported

// Endpoint Settings
#define IN_REPORT_EP      EP1
#define IN_REPORT_BDT     BD1_IN
#define IN_REPORT_EP_ADDR EP1_IN_BUFFER_BASE_ADDR

// Idle_Settings
#define DEFAULT_IDLE 500 // in mS

// HID Endpoint HAL
#define HID_EP      EP1
#define HID_EP_SIZE EP1_SIZE

#ifdef _PIC14E
#warning "HID EP Buffer addresses have been manually set for PIC16 devices."
#endif

#define HID_BD_OUT      BD1_OUT
#define HID_BD_OUT_EVEN BD1_OUT_EVEN
#define HID_BD_OUT_ODD  BD1_OUT_ODD
#define HID_BD_IN       BD1_IN
#define HID_BD_IN_EVEN  BD1_IN_EVEN
#define HID_BD_IN_ODD   BD1_IN_ODD

#define HID_UEPbits UEP1bits

/* NUMBER OF REPORTS */
#define HID_NUM_IN_REPORTS       1
#define HID_NUM_OUT_REPORTS      0
#define HID_NUM_FEATURE_REPORTS  0

/* REPORT STRUCTURES/VARS/DEFINES */
#define HID_USE_REPORT_IDS 0
#define HID_NUM_REPORT_IDS 0

// KEY MODIFIERS
#define MOD_KEY_LEFTCTRL    0x01 // LeftControl
#define MOD_KEY_LEFTSHIFT   0x02 // LeftShift
#define MOD_KEY_LEFTALT     0x04 // LeftAlt
#define MOD_KEY_LEFTMETA    0x08 // LeftGUI
#define MOD_KEY_RIGHTCTRL   0x10 // RightControl
#define MOD_KEY_RIGHTSHIFT  0x20 // RightShift
#define MOD_KEY_RIGHTALT    0x40 // RightAlt
#define MOD_KEY_RIGHTMETA   0x80 // RightGUI

// CONSUMER KEYS
#define _SCAN_NEXT_TRACK 0x01
#define _SCAN_PREV_TRACK 0x02
#define _STOP            0x04
#define _EJECT           0x08
#define _PLAY_PAUSE      0x10
#define _MUTE            0x20
#define _VOL_INC         0x40
#define _VOL_DEC         0x80

#endif /* USB_HID_CONFIG_H */