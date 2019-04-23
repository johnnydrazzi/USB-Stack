/**
 * @file hid_pages.h
 * @brief <i>Human Interface Device Class</i> pages.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - HID Library.
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
#ifndef HID_PAGES_H
#define HID_PAGES_H

/*
 * Meanings:
 * - LC - Linear Control
 * - OOC - On/Off Control
 * - MC - Momentary Control
 * - OSC - One Shot Control
 * - RTC - Re-Trigger Control
 * - SEL - Selector
 * - SV - Static Value
 * - SF - Static Flag
 * - DF - Dynamic Flag
 * - DV - Dynamic Value
 * - NAry - Named Array
 * - CA - Collection Application
 * - CL - Collection Logical
 * - CP - Collection Physical
 * - US - Usage Switch
 * - UM - Usage Modifier
 */

/******************************************************************************/
/***************************** GENERIC DESKTOP PAGE ***************************/
/******************************************************************************/
#define GENERIC_DESKTOP_PAGE            0x01

/* APPLICATION USAGES */
#define POINTER 0x01 // CP
#define MOUSE 0x02 // CA
// 0x03 Reserved
#define JOYSTICK 0x04 // CA
#define GAME_PAD 0x05 // CA
#define KEYBOARD 0x06 // CA
#define KEYPAD 0x07 // CA
#define MULTI_AXIS_CONTROLLER 0x08 // CA
#define TABLET_PC_SYS_CONTROLS 0x09 // CA
// 0x0A-0x2F Reserved

/* AXIS USAGES */
#define X 0x30 // DV
#define Y 0x31 // DV
#define Z 0x32 // DV
#define Rx 0x33 // DV
#define Ry 0x34 // DV
#define Rz 0x35 // DV

/* MISCELLANEOUS CONTROLS */
#define SLIDER 0x36 // DV
#define DIAL 0x37 // DV
#define WHEEL 0x38 // DV
#define HAT_SWITCH 0x39 // DV
#define MOTION_WAKEUP 0x3C // OSC
#define START 0x3D // OOC
#define SELECT 0x3E // OOC
// 0x3F Reserved
#define RESOLUTION_MULTIPLIER 0x48 // DV
// 0x49-0x7F Reserved

/* VECTOR USAGES */
#define Vx 0x40 // DV
#define Vy 0x41 // DV
#define Vz 0x42 // DV
#define Vbrx 0x43 // DV
#define Vbry 0x44 // DV
#define Vbrz 0x45 // DV
#define Vno 0x46 // DV

/* SYSTEM CONTROLS */
#define SYSTEM_CONTROL 0x80 // CA
#define SYSTEM_CONTEXT_MENU 0x84 // OSC
#define SYSTEM_MAIN_MENU 0x85 // OSC
#define SYSTEM_APP_MENU 0x86 // OSC
#define SYSTEM_MENU_HELP 0x87 // OSC
#define SYSTEM_MENU_EXIT 0x88 // OSC
#define SYSTEM_MENU_SELECT 0x89 // OSC
#define SYSTEM_MENU_RIGHT 0x8A // RTC
#define SYSTEM_MENU_LEFT 0x8B // RTC
#define SYSTEM_MENU_UP 0x8C // RTC
#define SYSTEM_MENU_DOWN 0x8D // RTC

/* POWER CONTROLS */
#define SYSTEM_POWER_DOWN 0x81 // OSC
#define SYSTEM_SLEEP 0x82 // OSC
#define SYSTEM_WAKE_UP 0x83 // OSC
#define SYSTEM_COLD_RESTART 0x8E // OSC
#define SYSTEM_WARM_RESTART 0x8F // OSC
#define SYSTEM_DOCK 0xA0 // OSC
#define SYSTEM_UNDOCK 0xA1 // OSC
#define SYSTEM_SETUP 0xA2 // OSC
#define SYSTEM_SPEAKER_MUTE 0xA7 // OSC
#define SYSTEM_HIBERNATE 0xA8 // OSC
// 0xA9-0xAF Reserved

/* BUFFERED BYTES */
#define COUNTED_BUFFER 0x3A // CL
#define BYTE_COUNT 0x3B // DV

/* DIRECTION PADS */
#define D_PAD_UP 0x90 // OSC
#define D_PAD_DOWN 0x91 // OSC
#define D_PAD_RIGHT 0x92 // OSC
#define D_PAD_LEFT 0x93 // OSC
// 0x94-0x9F Reserved

/* FEATURE_NOTIFICATION */
#define FEATURE_NOTIFICATION 0x47 // DV,DF

/* SOFTWARE FLOW CONTROLS */
#define SYSTEM_BREAK 0xA3 // OSC
#define SYSTEM_DEBUGGER_BREAK 0xA4 // OSC
#define APPLICATION_BREAK 0xA5 // OSC
#define APPLICATION_DEBUGGER_BREAK 0xA6 // OSC

/* SYSTEM DISPLAY CONTROLS */
#define SYSTEM_DISPLAY_INVERT 0xB0 // OSC
#define SYSTEM_DISPLAY_ITERNAL 0xB1 // OSC
#define SYSTEM_DISPLAY_EXTERNAL 0xB2 // OSC
#define SYSTEM_DISPLAY_BOTH 0xB3 // OSC
#define SYSTEM_DISPLAY_DUAL 0xB4 // OSC
#define SYSTEM_DISPLAY_TOGGLE_INT_EXT 0xB5 // OSC
#define SYSTEM_DISPLAY_SWAP_PRIM_SEC 0xB6 // OSC
#define SYSTEM_DISPLAY_LCD_AUTOSCALE 0xB7 // OSC

/******************************************************************************/
/************************* SIMULATION CONTROLS PAGE ***************************/
/******************************************************************************/

#define SIMULATION_CONTROLS_PAGE 0x02

/* SPORTS SIMULATION DEVICE */
#define SIM_SPORTS_SIMULATION_DEVICE 0x08 // CA

/* FLIGHT SIMULATION DEVICES */
#define SIM_FLIGHT_SIMULATION_DEVICE 0x01 // CA
#define SIM_SPACESHIP_SIMULATION_DEVICE 0x04 // CA
#define SIM_AIRPLANE_SIMULATION_DEVICE 0x09 // CA
#define SIM_HELICOPTOR_SIMULATION_DEVICE 0x0A // CA
#define SIM_AILERON 0xB0 // DV
#define SIM_AILERON_TRIM 0xB1 // DV
#define SIM_ANTI_TORQUE_CONTROL 0xB2 // DV
#define SIM_AUTO_PILOT_ENABLE 0xB3 // OOC
#define SIM_CHAFF_RELEASE 0xB4 // OSC
#define SIM_COLLECTIVE_CONTROL 0xB5 // DV
#define SIM_CYCLIC_CONTROL 0x22 // CP
#define SIM_CYCLIC_TRIM 0x23 // CP
#define SIM_DIVE_BRAKE 0xB6 // DV
#define SIM_ELECTRONIC_COUNTERMEASURES 0xB7 // OOC
#define SIM_ELEVATOR 0xB8 // DV
#define SIM_ELEVATOR_TRIM 0xB9 // DV
#define SIM_FLIGHT_COMMUNICATIONS 0xBC // OOC
#define SIM_FLARE_RELEASE 0xBD // OSC
#define SIM_FLIGHT_CONTROL_STICK 0x20 // CA
#define SIM_FLIGHT_STICK 0x21 // CA
#define SIM_LANDING_GEAR 0xBE // OOC
#define SIM_RUDDER 0xBA // DV
#define SIM_TOE_BRAKE 0xBF // DV
#define SIM_THROTTLE 0xBB // DV
#define SIM_TRIGGER 0xC0 // MC
#define SIM_WEAPONS_ARM 0xC1 // OOC
#define SIM_WEAPONS_SELECT 0xC2 // OSC
#define SIM_WING_FLAPS 0xC3 // DV
#define SIM_FLIGHT_YOKE 0x24 // CA

/* AUTOMOBILE SIMULATION DEVICES */
#define SIM_AUTOMOBILE_SIMULATION_DEVICE 0x02 // CA
#define SIM_ACCELERATOR 0xC4 // DV
#define SIM_BRAKE 0xC5 // DV
#define SIM_CLUTCH 0xC6 // DV
#define SIM_SHIFTER 0xC7 // DV
#define SIM_STEERING 0xC8 // DV

/* TANK SIMULATION DEVICES */
#define SIM_TANK_SIMULATION_DEVICE 0x03 // CA
#define SIM_TRACK_CONTROL 0x25 // CP
// 0x26-0xAF Reserved
#define SIM_TURRENT_DIRECTION 0xC9 // DV
#define SIM_BARREL_ELEVATION 0xCA // DV

/* MARITIME SIMULATION DEVICES */
#define SIM_SUBMARINE_SIMULATION_DEVICE 0x05 // CA
#define SIM_DIVE_PLANE 0xCB // DV
#define SIM_BALLAST 0xCC // DV
#define SIM_SAILING_SIMULATION_DEVICE 0x06 // CA

/* TWO-WHEELED SIMULATION DEVICES */
#define SIM_MOTORCYCLE_SIMULATION_DEVICE 0x07 // CA
#define SIM_BICYCLE_SIMULATION_DEVICE 0x0C // CA
// 0x0D-0x1F Reserved
#define SIM_BICYCLE_CRANK 0xCD // DV
#define SIM_HANDLE_BARS 0xCE // DV
#define SIM_FRONT_BRAKE 0xCF // DV
#define SIM_REAR_BRAKE 0xD0 // DV
// 0xD1-0xFFFF Reserved

/* MISCELLANEOUS SIMULATION DEVICES */
#define SIM_MAGIC_CARPER_SIMULATION_DEVICE 0x0B // CA
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#define VR_CONTROLS_PAGE 0x03
#define SPORT_CONTROLS_PAGE 0x04
#define GAME_CONTROLS_PAGE 0x05
#define GENERIC_DEVICE_CONTROLS_PAGE 0x06

/******************************************************************************/
/***************************** KEYBOARD KEYPAD PAGE ***************************/
/******************************************************************************/
#define KEYBOARD_KEYPAD_PAGE 0x07

/* KEY SCANCODES */
#define KEY_NULL 0x00 // No event.
#define KEY_ROLLOVER 0x01
#define KEY_POST_FAIL 0x02
#define KEY_ERR_UNDEF 0x03
#define KEY_A 0x04 // a and A
#define KEY_B 0x05 // b and B
#define KEY_C 0x06 // c and C
#define KEY_D 0x07 // b and B
#define KEY_E 0x08 // e and E
#define KEY_F 0x09 // f and F
#define KEY_G 0x0A // g and G
#define KEY_H 0x0B // h and H
#define KEY_I 0x0C // i and I
#define KEY_J 0x0D // j and J
#define KEY_K 0x0E // k and K
#define KEY_L 0x0F // l and L
#define KEY_M 0x10 // m and M
#define KEY_N 0x11 // n and N
#define KEY_O 0x12 // o and O
#define KEY_P 0x13 // p and P
#define KEY_Q 0x14 // q and Q
#define KEY_R 0x15 // r and R
#define KEY_S 0x16 // s and S
#define KEY_T 0x17 // t and T
#define KEY_U 0x18 // u and U
#define KEY_V 0x19 // v and V
#define KEY_W 0x1A // w and W
#define KEY_X 0x1B // x and X
#define KEY_Y 0x1C // y and Y
#define KEY_Z 0x1D // z and Z
#define KEY_1 0x1E // 1 and !
#define KEY_2 0x1F // 2 and @
#define KEY_3 0x20 // 3 and #
#define KEY_4 0x21 // 4 and $
#define KEY_5 0x22 // 5 and %
#define KEY_6 0x23 // 6 and ^
#define KEY_7 0x24 // 7 and &
#define KEY_8 0x25 // 8 and *
#define KEY_9 0x26 // 9 and (
#define KEY_0 0x27 // 0 and )
#define KEY_ENTER 0x28 // Return (ENTER)
#define KEY_ESC 0x29 // ESCAPE
#define KEY_BACKSPACE 0x2A // DELETE (Backspace)
#define KEY_TAB 0x2B // Tab
#define KEY_SPACE 0x2C // Spacebar
#define KEY_MINUS 0x2D // - and _
#define KEY_EQUAL 0x2E // = and +
#define KEY_LEFTBRACE 0x2F // [ and {
#define KEY_RIGHTBRACE 0x30 // ] and }
#define KEY_BACKSLASH 0x31 // \ and |
#define KEY_HASHTILDE 0x32 // Non-US # and ~
#define KEY_SEMICOLON 0x33 // ; and :
#define KEY_APOSTROPHE 0x34 // ' and "
#define KEY_GRAVE 0x35 // ` and ~
#define KEY_COMMA 0x36 // , and <
#define KEY_DOT 0x37 // . and >
#define KEY_SLASH 0x38 // / and ?
#define KEY_CAPSLOCK 0x39 // Caps Lock
#define KEY_F1 0x3A // F1
#define KEY_F2 0x3B // F2
#define KEY_F3 0x3C // F3
#define KEY_F4 0x3D // F4
#define KEY_F5 0x3E // F5
#define KEY_F6 0x3F // F6
#define KEY_F7 0x40 // F7
#define KEY_F8 0x41 // F8
#define KEY_F9  0x42 // F9
#define KEY_F10 0x43 // F10
#define KEY_F11 0x44 // F11
#define KEY_F12 0x45 // F12
#define KEY_SYSRQ 0x46 // PrintScreen
#define KEY_SCROLLLOCK 0x47 // Scroll Lock
#define KEY_PAUSE 0x48 // Pause
#define KEY_INSERT 0x49 // Insert
#define KEY_HOME 0x4A // Home
#define KEY_PAGEUP 0x4B // PageUp
#define KEY_DELETE 0x4C // Delete Forward
#define KEY_END 0x4D // End
#define KEY_PAGEDOWN 0x4E // PageDown
#define KEY_RIGHT 0x4F // RightArrow
#define KEY_LEFT 0x50 // LeftArrow
#define KEY_DOWN 0x51 // DownArrow
#define KEY_UP 0x52 // UpArrow
#define KEY_NUMLOCK 0x53 // Num Lock and Clear
#define KEY_KPSLASH 0x54 // Keypad /
#define KEY_KPASTERISK 0x55 // Keypad *
#define KEY_KPMINUS 0x56 // Keypad -
#define KEY_KPPLUS 0x57 // Keypad +
#define KEY_KPENTER 0x58 // Keypad ENTER
#define KEY_KP1 0x59 // Keypad 1 and End
#define KEY_KP2 0x5A // Keypad 2 and DownArrow
#define KEY_KP3 0x5B // Keypad 3 and PageDn
#define KEY_KP4 0x5C // Keypad 4 and Left Arrow
#define KEY_KP5 0x5D // Keypad 5
#define KEY_KP6 0x5E // Keypad 6 and Right Arrow
#define KEY_KP7 0x5F // Keypad 7 and Home
#define KEY_KP8 0x60 // Keypad 8 and Up Arrow
#define KEY_KP9 0x61 // Keypad 9 and PageUp
#define KEY_KP0 0x62 // Keypad 0 and Insert
#define KEY_KPDOT 0x63 // Keypad . and Delete
#define KEY_102ND 0x64 // Non-US \ and |
#define KEY_COMPOSE 0x65 // Application
#define KEY_POWER 0x66 // Power
#define KEY_KPEQUAL 0x67 // Keypad =
#define KEY_F13 0x68 // F13
#define KEY_F14 0x69 // F14
#define KEY_F15 0x6A // F15
#define KEY_F16 0x6B // F16
#define KEY_F17 0x6C // F17
#define KEY_F18 0x6D // F18
#define KEY_F19 0x6E // F19
#define KEY_F20 0x6F // F20
#define KEY_F21 0x70 // F21
#define KEY_F22 0x71 // F22
#define KEY_F23 0x72 // F23
#define KEY_F24 0x73 // F24
#define KEY_OPEN 0x74 // Execute
#define KEY_HELP 0x75 // Help
#define KEY_PROPS 0x76 // Menu
#define KEY_FRONT 0x77 // Select
#define KEY_STOP 0x78 // Stop
#define KEY_AGAIN 0x79 // Again
#define KEY_UNDO 0x7A // Undo
#define KEY_CUT 0x7B // Cut
#define KEY_COPY 0x7C // Copy
#define KEY_PASTE 0x7D // Past
#define KEY_FIND 0x7E // Find
#define KEY_MUTE 0x7F // Mute
#define KEY_VOLUMEUP 0x80 // Volume Up
#define KEY_VOLUMEDOWN 0x81 // Volume Down
#define KEY_LOCKINGCAPSLOCK 0x82 // Locking Caps Lock
#define KEY_LOCKINGNUMLOCK 0x83 // Locking Num Lock
#define KEY_LOCKINGSCROLLLOCK 0x84 // Locking Scroll Lock
#define KEY_KPCOMMA 0x85 // Keypad ,
#define KEY_KPEQUAL_AS400 0x86 // Keypad = for AS/400
#define KEY_INTERNATIONAL1 0x87 // International1
#define KEY_INTERNATIONAL2 0x88 // International2
#define KEY_INTERNATIONAL3 0x89 // International3
#define KEY_INTERNATIONAL4 0x8A // International4
#define KEY_INTERNATIONAL5 0x8B // International5
#define KEY_INTERNATIONAL6 0x8C // International6
#define KEY_INTERNATIONAL7 0x8D // International7
#define KEY_INTERNATIONAL8 0x8E // International8
#define KEY_INTERNATIONAL9 0x8F // International9
#define KEY_LANG1 0x90 // LANG1
#define KEY_LANG2 0x91 // LANG2
#define KEY_LANG3 0x92 // LANG3
#define KEY_LANG4 0x93 // LANG4
#define KEY_LANG5 0x94 // LANG5
#define KEY_LANG6 0x95 // LANG6
#define KEY_LANG7 0x96 // LANG7
#define KEY_LANG8 0x97 // LANG8
#define KEY_LANG9 0x98 // LANG9
#define KEY_ALTERNATE_ERASE 0x99 // Alternate Erase
#define KEY_SYSREQ_ATTENTION 0x9A // SysReq/Attention
#define KEY_CANCEL 0x9B // Cancel
#define KEY_CLEAR 0x9C // Clear
#define KEY_PRIOR 0x9D // Prior
#define KEY_RETURN 0x9E // Return
#define KEY_SEPARATOR 0x9F // Separator
#define KEY_OUT 0xA0 // Out
#define KEY_OPER 0xA1 // Oper
#define KEY_CLEAR_AGAIN 0xA2 // Clear/Again
#define KEY_CRSEL_PROPS 0xA3 // CrSel/Props
#define KEY_EXSEL 0xA4 // ExSel
// 0xA5 - 0xAF // Reserved
#define KEY_KP00 0xB0 // Keypad 00
#define KEY_KP000 0xB1 // Keypad 000
#define KEY_KPTHOUSEP 0xB2 // Thousands Separator
#define KEY_KPDECSEP 0xB3 // Decimal Separator
#define KEY_KPCURRUNIT 0xB4 // Currency Unit
#define KEY_KPCURRSUBUNIT 0xB5 // Currency Sub-unit
#define KEY_KPLEFTPAREN 0xB6 // Keypad (
#define KEY_KPRIGHTPAREN 0xB7 // Keypad )
// 0xB8 // Keypad {
// 0xB9 // Keypad }
// 0xBA // Keypad Tab
// 0xBB // Keypad Backspace
// 0xBC // Keypad A
// 0xBD // Keypad B
// 0xBE // Keypad C
// 0xBF // Keypad D
// 0xC0 // Keypad E
// 0xC1 // Keypad F
// 0xC2 // Keypad XOR
// 0xC3 // Keypad ^
// 0xC4 // Keypad %
// 0xC5 // Keypad <
// 0xC6 // Keypad >
// 0xC7 // Keypad &
// 0xC8 // Keypad &&
// 0xC9 // Keypad |
// 0xCA // Keypad ||
// 0xCB // Keypad :
// 0xCC // Keypad #
// 0xCD // Keypad Space
// 0xCE // Keypad @
// 0xCF // Keypad !
// 0xD0 // Keypad Memory Store
// 0xD1 // Keypad Memory Recall
// 0xD2 // Keypad Memory Clear
// 0xD3 // Keypad Memory Add
// 0xD4 // Keypad Memory Subtract
// 0xD5 // Keypad Memory Multiply
// 0xD6 // Keypad Memory Divide
// 0xD7 // Keypad +/-
// 0xD8 // Keypad Clear
// 0xD9 // Keypad Clear Entry
// 0xDA // Keypad Binary
// 0xDB // Keypad Octal
// 0xDC // Keypad Decimal
// 0xDD // Keypad Hexadecimal
// 0xDE // Reserved
// 0xDF // Reserved
#define KEY_LEFTCTRL 0xE0 // LeftControl
#define KEY_LEFTSHIFT 0xE1 // LeftShift
#define KEY_LEFTALT 0xE2 // LeftAlt
#define KEY_LEFTMETA 0xE3 // LeftGUI
#define KEY_RIGHTCTRL 0xE4 // RightControl
#define KEY_RIGHTSHIFT 0xE5 // RightShift
#define KEY_RIGHTALT 0xE6 // RightAlt
#define KEY_RIGHTMETA 0xE7 // RightGUI

/******************************************************************************/
/*********************************** LED PAGE *********************************/
/******************************************************************************/
#define LED_PAGE 0x08

/* KEYBOARD INDICATORS */
#define LED_NUM_LOCK 0x01 // OOC
#define LED_CAPS_LOCK 0x02 // OOC
#define LED_SCROLL_LOCK 0x03 // OOC
#define LED_COMPOSE 0x04 // OOC
#define LED_KANA 0x05 // OOC
#define LED_SHIFT 0x07 // OOC

/* TELEPHONY INDICATORS */
#define LED_DO_NOT_DISTURB 0x08 // OOC
#define LED_OFF_HOOK 0x17 // OOC
#define LED_RING 0x18 // OOC
#define LED_MESSAGE_WAITING 0x19 // OOC
#define LED_DATA_MODE 0x1A // OOC
#define LED_SPEAKER 0x1E // OOC
#define LED_HEAD_SET 0x1F // OOC
#define LED_HOLD 0x20 // OOC
#define LED_MICROPHONE 0x21 // OOC
#define LED_COVERAGE 0x22 // OOC
#define LED_NIGHT_MODE 0x23 // OOC
#define LED_SEND_CALLS 0x24 // OOC
#define LED_CALL_PICKUP 0x25 // OOC
#define LED_CONFERENCE 0x26 // OOC

/* CONSUMER INDICATORS */
#define LED_MUTE 0x09 // OOC
#define LED_TONE_ENABLE 0x0A // OOC
#define LED_HIGH_CUT_FILTER 0x0B // OOC
#define LED_LOW_CUT_FILTER 0x0C // OOC
#define LED_EQUALIZER_ENABLE 0x0D // OOC
#define LED_SOUND_FIELD_ON 0x0E // OOC
#define LED_SURROUND_ON 0x0F // OOC
#define LED_REPEAT 0x10 // OOC
#define LED_STEREO 0x11 // OOC
#define LED_SAMPLING_RATE_DETECT 0x12 // OOC
#define LED_CAV 0x14 // OOC
#define LED_CLV 0x15 // OOC
#define LED_CAMERA_ON 0x28 // OOC
#define LED_CAMERA_OFF 0x29 // OOC

/* MEDIA TRANSPORT INDICATORS */
#define LED_SPINNING 0x13 // OOC
#define LED_RECORD_FORMAT_DETECT 0x16 // OOC
#define LED_STOP 0x33 // OOC
#define LED_FORWARD 0x31 // OOC
#define LED_REVERSE 0x32 // OOC
#define LED_REWIND 0x34 // OOC
#define LED_FAST_FORWARD 0x35 // OOC
#define LED_PLAY 0x36 // OOC
#define LED_PAUSE 0x37 // OOC
#define LED_RECORD 0x38 // OOC

/* PRINTER INDICATORS */
#define LED_PAPER_OUT 0x2E // OOC
#define LED_PAPER_JAM 0x2F // OOC

/* GENERAL DEVICE INDICATORS */
#define LED_POWER 0x06 // OOC
#define LED_STANDBY 0x27 // OOC
#define LED_ONLINE 0x2A // OOC
#define LED_OFFLINE 0x2B // OOC
#define LED_BUSY 0x2C // OOC
#define LED_READY 0x2D // OOC
#define LED_REMOTE 0x30 // OOC
#define LED_ERROR 0x39 // OOC
#define LED_BATTERY_OPERATION 0x1B // OOC
#define LED_BATTERY_OK 0x1C // OOC
#define LED_BATTERY_LOW 0x1D // OOC
#define LED_USAGE_SELECTED_INDICATOR 0x3A // US
#define LED_USAGE_IN_USE_INDICATOR 0x3B // US
#define LED_USAGE_MULTI_MODE_INDICATOR 0x3C // UM
#define LED_INDICATOR_ON 0x3D // SEL
#define LED_INDICATOR_FLASH 0x3E // SEL
#define LED_INDICATOR_SLOW_BLINK 0x3F // SEL
#define LED_INDICATOR_FAST_BLINK 0x40 // SEL
#define LED_INDICATOR_OFF 0x41 // SEL
#define LED_FLASH_ON_TIME 0x42 // DV
#define LED_SLOW_BLINK_ON_TIME 0x43 // DV
#define LED_SLOW_BLINK_OFF_TIME 0x44 // DV
#define LED_FAST_BLINK_ON_TIME 0x45 // DV
#define LED_FAST_BLINK_OFF_TIME 0x46 // DV
#define LED_USAGE_INDICATOR_COLOR 0x47 // UM
#define LED_INDICATOR_RED 0x48 // SEL
#define LED_INDICATOR_GREEN 0x49 // SEL
#define LED_INDICATOR_AMBER 0x4A // SEL
#define LED_GENERIC_INDICATOR 0x4B // OOC
#define LED_SYSTEM_SUSPEND 0x4C // OOC
#define LED_EXTERNAL_POWER_CONNECTED 0x4D // OOC
// 0x4E-0xFFFF Reserved
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#define BUTTON_PAGE 0x09
#define ORDINAL_PAGE 0x0A
#define TELEPHONY_DEVICE_PAGE 0x0B


/******************************************************************************/
/********************************* CONSUMER PAGE ******************************/
/******************************************************************************/
#define CONSUMER_PAGE 0x0C

/* GENERIC CONSUMER CONTROL DEVICE */
#define CONSUMER_CONSUMER_CONTROL 0x01 // CA
#define CONSUMER_MICROPHONE 0x04 // CA
#define CONSUMER_HEADPHONE 0x05 // CA
#define CONSUMER_GRAPHIC_EQUALIZER 0x06 // CA
/* 0x07-0x1F Reserved */

/* NUMERIC KEY PAD */
#define CONSUMER_NUMERIC_KEY_PAD 0x02 // NAry
#define CONSUMER_PLUS_10 0x20 // OSC
#define CONSUMER_PLUS_100 0x21 // OSC
#define CONSUMER_AM_PM 0x22 // OSC
/* 0x23-0x2F Reserved */

/* GENERAL CONTROLS */
#define CONSUMER_POWER 0x30 // OSC
#define CONSUMER_RESET 0x31 // OSC
#define CONSUMER_SLEEP 0x32 // OSC
#define CONSUMER_SLEEP_AFTER 0x33 // OSC
#define CONSUMER_SLEEP_MODE 0x34 // RTC
#define CONSUMER_ILLUMINATION 0x35 // OOC
#define CONSUMER_FUNCTION_BUTTONS 0x36 // NAry
/* 0x37-0x3F Reserved */

/* MENU CONTROLS */
#define CONSUMER_MENU 0x40 // OOC
#define CONSUMER_MENU_PICK 0x41 // OSC
#define CONSUMER_MENU_UP 0x42 // OSC
#define CONSUMER_MENU_DOWN 0x43 // OSC
#define CONSUMER_MENU_LEFT 0x44 // OSC
#define CONSUMER_MENU_RIGHT 0x45 // OSC
#define CONSUMER_MENU_ESCAPE 0x46 // OSC
#define CONSUMER_MENU_VALUE_INCREASE 0x47 // OSC
#define CONSUMER_MENU_VALUE_DECREASE 0x48 // OSC
/* 0x49-0x5F Reserved */

/* DISPLAY CONTROLS */
#define CONSUMER_DATA_ON_SCREEN 0x60 // OOC
#define CONSUMER_CLOSED_CAPTION 0x61 // OOC
#define CONSUMER_CLOSED_CAPTION_SELECT 0x62 // OSC
#define CONSUMER_VCR_TV 0x63 // OOC
#define CONSUMER_BROADCAST_MODE 0x64 // OSC
#define CONSUMER_SNAPSHOT 0x65 // OSC
#define CONSUMER_STILL 0x66 // OSC
/* 0x67-0x7F Reserved */

/* SELECTION CONTROLS */
#define CONSUMER_SELECTION 0x80 // NAry
#define CONSUMER_ASSIGN_SELECTION 0x81 // OSC
#define CONSUMER_MODE_STEP 0x82 // OSC
#define CONSUMER_RECALL_LAST 0x83 // OSC
#define CONSUMER_ENTER_CHANNEL 0x84 // OSC
#define CONSUMER_ORDER_MOVIE 0x85 // OSC
#define CONSUMER_CHANNEL 0x86 // LC
#define CONSUMER_CHANNEL_INCREMENT 0x9C // OSC
#define CONSUMER_CHANNEL_DECREMENT 0x9D // OSC
#define CONSUMER_VCR_PLUS 0xA0 // OSC
#define CONSUMER_ONCE 0xA1 // OSC
#define CONSUMER_DAILY 0xA2 // OSC
#define CONSUMER_WEEKLY 0xA3 // OSC
#define CONSUMER_MONTHLY 0xA4 // OSC
/* 0xA5-0xAF Reserved */
#define CONSUMER_MEDIA_SELECTION 0x87 // SEL
#define CONSUMER_MEDIA_SELECT_COMPUTER 0x88 // SEL
#define CONSUMER_MEDIA_SELECT_TV 0x89 // SEL
#define CONSUMER_MEDIA_SELECT_WWW 0x8A // SEL
#define CONSUMER_MEDIA_SELECT_DVD 0x8B // SEL
#define CONSUMER_MEDIA_SELECT_TELEPHONE 0x8C // SEL
#define CONSUMER_MEDIA_SELECT_PROGRAM_GUIDE 0x8D // SEL
#define CONSUMER_MEDIA_SELECT_VIDEO_PHONE 0x8E // SEL
#define CONSUMER_MEDIA_SELECT_GAMES 0x8F // SEL
#define CONSUMER_MEDIA_SELECT_MESSAGES 0x90 // SEL
#define CONSUMER_MEDIA_SELECT_CD 0x91 // SEL
#define CONSUMER_MEDIA_SELECT_VCR 0x92 // SEL
#define CONSUMER_MEDIA_SELECT_TUNER 0x93 // SEL
#define CONSUMER_MEDIA_SELECT_TAPE 0x96 // SEL
#define CONSUMER_MEDIA_SELECT_CABLE 0x97 // SEL
#define CONSUMER_MEDIA_SELECT_SATELLITE 0x98 // SEL
#define CONSUMER_MEDIA_SELECT_SECURITY 0x99 // SEL
#define CONSUMER_MEDIA_SELECT_HOME 0x9A // SEL
#define CONSUMER_MEDIA_SELECT_CALL 0x9B // SEL
#define CONSUMER_QUIT 0x94 // OSC
#define CONSUMER_HELP 0x95 // OSC

/* TRANSPORT CONTROLS */
#define CONSUMER_PLAY 0xB0 // OOC
#define CONSUMER_PAUSE 0xB1 // OOC
#define CONSUMER_RECORD 0xB2 // OOC
#define CONSUMER_FAST_FORWARD 0xB3 // OOC
#define CONSUMER_REWIND 0xB4 // OOC
#define CONSUMER_SCAN_NEXT_TRACK 0xB5 // OSC
#define CONSUMER_SCAN_PREVIOUS_TRACK 0xB6 // OSC
#define CONSUMER_STOP 0xB7 // OSC
#define CONSUMER_EJECT 0xB8 // OSC
#define CONSUMER_STOP_EJECT 0xCC // OSC
#define CONSUMER_PLAY_PAUSE 0xCD // OSC
#define CONSUMER_PLAY_SKIP 0xCE // OSC
/* 0xCF-0xDF Reserved */
#define CONSUMER_RANDOM_PLAY 0xB9 // OOC
#define CONSUMER_REPEAT 0xBC // OSC
#define CONSUMER_SELECT_DISC 0xBA // NAry
#define CONSUMER_ENTER_DISC 0xBB // MC
#define CONSUMER_TRACKING 0xBD // LC
#define CONSUMER_TRACKING_INCREMENT 0xCA // RTC
#define CONSUMER_TRACKING_DECREMENT 0xCB // RTC
#define CONSUMER_TRACK_NORMAL 0xBE // OSC
#define CONSUMER_SLOW_TRACKING 0xBF // LC
#define CONSUMER_FRAME_FORWARD 0xC0 // RTC
#define CONSUMER_FRAME_BACK 0xC1 // RTC

/* SEARCH CONTROLS */
#define CONSUMER_MARK 0xC2 // OSC
#define CONSUMER_CLEAR_MARK 0xC3 // OSC
#define CONSUMER_REPEAT_FROM_MARK 0xC4 // OOC
#define CONSUMER_RETURN_TO_MARK 0xC5 // OSC
#define CONSUMER_SEARCH_MARK_FORWARD 0xC6 // OSC
#define CONSUMER_SEARCH_MARK_BACKWARDS 0xC7 // OSC
#define CONSUMER_COUNTER_RESET 0xC8 // OSC
#define CONSUMER_SHOW_COUNTER 0xC9 // OSC

/* AUDIO CONTROLS */
#define CONSUMER_VOLUME 0xE0 // LC
#define CONSUMER_VOLUME_INCREMENT 0xE9 // RTC
#define CONSUMER_VOLUME_DECREMENT 0xEA // RTC
/* 0xEB-0xEF Reserved */
#define CONSUMER_MUTE 0xE2 // OOC
#define CONSUMER_BALANCE 0xE1 // LC
#define CONSUMER_BALANCE_RIGHT 0x150 // RTC
#define CONSUMER_BALANCE_LEFT 0x151 // RTC
#define CONSUMER_BASS 0xE3 // LC
#define CONSUMER_BASS_INCREMENT 0x152 // RTC
#define CONSUMER_BASS_DECREMENT 0x153 // RTC
#define CONSUMER_BASS_BOOST 0xE5 // OOC
#define CONSUMER_TREBLE 0xE4 // LC
#define CONSUMER_TREBLE_INCREMENT 0x154 // RTC
#define CONSUMER_TREBLE_DECREMENT 0x155 // RTC
/* 0x156-0x15F Reserved */
#define CONSUMER_SURROUND_MODE 0xE6 // OSC
#define CONSUMER_LOUDNESS 0xE7 // OOC
#define CONSUMER_MPX 0xE8 // OOC

/* SPEED CONTROLS */
#define CONSUMER_SPEED_SELECT 0xF0 // OSC
#define CONSUMER_PLAYBACK_SPEED 0xF1 // NAry
#define CONSUMER_STANDARD_PLAY 0xF2 // SEL
#define CONSUMER_LONG_PLAY 0xF3 // SEL
#define CONSUMER_EXTENDED_PLAY 0xF4 // SEL
#define CONSUMER_SLOW 0xF5 // OSC
/* 0xF6-0xFF Reserved */

/* HOME AND SECURITY CONTROLS */
#define CONSUMER_FAN_ENABLE 0x100 // OOC
#define CONSUMER_FAN_SPEED 0x101 // LC
#define CONSUMER_LIGHT_ENABLE 0x102 // OOC
#define CONSUMER_LIGHT_ILLUMINATION_LEVEL 0x103 // LC
#define CONSUMER_CLIMATE_CONTROL_ENABLE 0x104 // OOC
#define CONSUMER_ROOM_TEMPERATURE 0x105 // LC
#define CONSUMER_SECURITY_ENABLE 0x106 // OOC
#define CONSUMER_FIRE_ALARM 0x107 // OSC
#define CONSUMER_POLICE_ALARM 0x108 // OSC
#define CONSUMER_PROXIMITY 0x109 // LC
#define CONSUMER_MOTION 0x10A // OSC
#define CONSUMER_DURESS_ALARM 0x10B // OSC
#define CONSUMER_HOLDUP_ALARM 0x10C // OSC
#define CONSUMER_MEDICAL_ALARM 0x10D // OSC
/* 0x10E-0x14F Reserved */

/* SPEAKER CHANNELS */
#define CONSUMER_SPEAKER_SYSTEM 0x160 // CL
#define CONSUMER_CHANNEL_LEFT 0x161 // CL
#define CONSUMER_CHANNEL_RIGHT 0x162 // CL
#define CONSUMER_CHANNEL_CENTER 0x163 // CL
#define CONSUMER_CHANNEL_FRONT 0x164 // CL
#define CONSUMER_CHANNEL_CENTER_FRONT 0x165 // CL
#define CONSUMER_CHANNEL_SIDE 0x166 // CL
#define CONSUMER_CHANNEL_SURROUND 0x167 // CL
#define CONSUMER_CHANNEL_LOW_FREQUENCY_ENHANCEMENT 0x168 // CL
#define CONSUMER_CHANNEL_TOP 0x169 // CL
#define CONSUMER_CHANNEL_UNKNOWN 0x16A // CL
/* 0x16B-0x16F Reserved */

/* PC THEATRE */
#define CONSUMER_MEDIA_SELECT_SAP 0x9E // SEL
/* 0x9F Reserved */
#define CONSUMER_SUB_CHANNEL 0x170 // LC
#define CONSUMER_SUB_CHANNEL_INCREMENT 0x171 //OSC
#define CONSUMER_SUB_CHANNEL_DECREMENT 0x172 //OSC
#define CONSUMER_ALTERNATE_AUDIO_INCREMENT 0x173 //OSC
#define CONSUMER_ALTERNATE_AUDIO_DECREMENT 0x174 //OSC
/* 0x175-0x17F Reserved */

/* PROGRAMMABLE_BUTTONS */
#define CONSUMER_PROGRAMMABLE_BUTTONS  0x03 // NAry

/* APPLICATION LAUNCH BUTTONS */
#define CONSUMER_APPLICATION_LAUNCH_BUTTONS 0x180 // NAry
#define CONSUMER_AL_LAUNCH_BUTTON_CONFIGURATION_TOOL 0x181 // SEL
#define CONSUMER_AL_PROGRAMMABLE_BUTTON_CONFIGURATION 0x182 // SEL
#define CONSUMER_AL_CONSUMER_CONTROL_CONFIGURATION 0x183 // SEL
#define CONSUMER_AL_WORD_PROCESSOR 0x184 // SEL
#define CONSUMER_AL_TEXT_EDITOR 0x185 // SEL
#define CONSUMER_AL_SPREADSHEET 0x186 // SEL
#define CONSUMER_AL_GRAPHICS_EDITOR 0x187 // SEL
#define CONSUMER_AL_PRESENTATION_APP 0x188 // SEL
#define CONSUMER_AL_DATABASE_APP 0x189 // SEL
#define CONSUMER_AL_EMAIL_READER 0x18A // SEL
#define CONSUMER_AL_NEWSREADER 0x18B // SEL
#define CONSUMER_AL_VOICEMAIL 0x18C // SEL
#define CONSUMER_AL_CONTACTS_ADDRESS_BOOK 0x18D // SEL
#define CONSUMER_AL_CALENDAR_SCHEDULE 0x18E // SEL
#define CONSUMER_AL_TASK_PROJECT_MANAGER 0x18F // SEL
#define CONSUMER_AL_LOG_JOURNAL_TIMECARD 0x190 // SEL
#define CONSUMER_AL_CHECKBOOK_FINANCE 0x191 // SEL
#define CONSUMER_AL_CALCULATOR 0x192 // SEL
#define CONSUMER_AL_A_V_CAPTURE_PLAYBACK 0x193 // SEL
#define CONSUMER_AL_LOCAL_MACHINE_BROWSER 0x194 // SEL
#define CONSUMER_AL_LAN_WAN_BROWSER 0x195 // SEL
#define CONSUMER_AL_INTERNET_BROWSER 0x196 // SEL
#define CONSUMER_AL_REMOTE_NETWORKING_ISP_CONNECT 0x197 // SEL
#define CONSUMER_AL_NETWORK_CONFERENCE 0x198 // SEL
#define CONSUMER_AL_NETWORK_CHAT 0x199 // SEL
#define CONSUMER_AL_TELEPHONY_DIALER 0x19A // SEL
#define CONSUMER_AL_LOGON 0x19B // SEL
#define CONSUMER_AL_LOGOFF 0x19C // SEL
#define CONSUMER_AL_LOGON_LOGOFF 0x19D // SEL
#define CONSUMER_AL_TERMINAL_LOCK_SCREENSAVER 0x19E // SEL
#define CONSUMER_AL_CONTROL_PANEL 0x19F // SEL
#define CONSUMER_AL_COMMAND_LINE_PROCESSOR_RUN 0x1A0 // SEL
#define CONSUMER_AL_PROCESS_TASK_MANAGER 0x1A1 // SEL
#define CONSUMER_AL_SELECT_TASK_APPLICATION 0x1A2 // SEL
#define CONSUMER_AL_NEXT_TASK_APPLICATION 0x1A3 // SEL
#define CONSUMER_AL_PREVIOUS_TASK_APPLICATION 0x1A4 // SEL
#define CONSUMER_AL_PREEMPTIVE_HALT_TASK_APPLICATION 0x1A5 // SEL
#define CONSUMER_AL_INTEGRATED_HELP_CENTER 0x1A6 // SEL
#define CONSUMER_AL_POWER_STATUS 0x1B5 // SEL
#define CONSUMER_AL_DOCUMENTS 0x1A7 // SEL
#define CONSUMER_AL_THESAURUS 0x1A8 // SEL
#define CONSUMER_AL_DICTIONARY 0x1A9 // SEL
#define CONSUMER_AL_DESKTOP 0x1AA // SEL
#define CONSUMER_AL_SPELL_CHECK 0x1AB // SEL
#define CONSUMER_AL_GRAMMAR_CHECK 0x1AC // SEL
#define CONSUMER_AL_WIRELESS_STATUS 0x1AD // SEL
#define CONSUMER_AL_KEYBOARD_LAYOUT 0x1AE // SEL
#define CONSUMER_AL_VIRUS_PROTECTION 0x1AF // SEL
#define CONSUMER_AL_ENCRYPTION 0x1B0 // SEL
#define CONSUMER_AL_SCREEN_SAVER 0x1B1 // SEL
#define CONSUMER_AL_ALARMS 0x1B2 // SEL
#define CONSUMER_AL_CLOCK 0x1B3 // SEL
#define CONSUMER_AL_FILE_BROWSER 0x1B4 // SEL
#define CONSUMER_AL_IMAGE_BROWSER 0x1B6 // SEL
#define CONSUMER_AL_AUDIO_BROWSER 0x1B7 // SEL
#define CONSUMER_AL_MOVIE_BROWSER 0x1B8 // SEL
#define CONSUMER_AL_DIGITAL_RIGHTS_MANAGER 0x1B9 // SEL
#define CONSUMER_AL_DIGITAL_WALLET 0x1BA // SEL
/* 0x1BB Reserved */
#define CONSUMER_AL_INSTANT_MESSAGING 0x1BC // SEL
#define CONSUMER_AL_OEM_FEATURES_BROWSER 0x1BD // SEL
#define CONSUMER_AL_OEM_HELP 0x1BE // SEL
#define CONSUMER_AL_ONLINE_COMMUNITY 0x1BF // SEL
#define CONSUMER_AL_ENTERTAINMENT_CONTENT_BROWSER 0x1C0 // SEL
#define CONSUMER_AL_ONLINE_SHOPPING_BROWSER 0x1C1 // SEL
#define CONSUMER_AL_SMARTCARD_INFORMATION_HELP 0x1C2 // SEL
#define CONSUMER_AL_MARKET_MONITOR_FINANCE_BROWSER 0x1C3 // SEL
#define CONSUMER_AL_CUSTOMIZED_CORPORATE_NEWS_BROWSER 0x1C4 // SEL
#define CONSUMER_AL_ONLINE_ACTIVITY_BROWSER 0x1C5 // SEL
#define CONSUMER_AL_RESEARCH_SEARCH_BROWSER 0x1C6 // SEL
#define CONSUMER_AL_AUDIO_PLAYER 0x1C7 // SEL
/* 0x1C8-0x1FF Reserved */

/* GENERIC GUI APPLICATION CONTROLS */
#define CONSUMER_GENERIC_GUI_APPLICATION_CONTROLS 0x200 // NAry
#define CONSUMER_AC_NEW 0x201 // SEL
#define CONSUMER_AC_OPEN 0x202 // SEL
#define CONSUMER_AC_CLOSE 0x203 // SEL
#define CONSUMER_AC_EXIT 0x204 // SEL
#define CONSUMER_AC_MAXIMIZE 0x205 // SEL
#define CONSUMER_AC_MINIMIZE 0x206 // SEL
#define CONSUMER_AC_SAVE 0x207 // SEL
#define CONSUMER_AC_PRINT 0x208 // SEL
#define CONSUMER_AC_PROPERTIES 0x209 // SEL
/* 0x20A-0x219 Reserved */
#define CONSUMER_AC_UNDO 0x21A // SEL
#define CONSUMER_AC_COPY 0x21B // SEL
#define CONSUMER_AC_CUT 0x21C // SEL
#define CONSUMER_AC_PASTE 0x21D // SEL
#define CONSUMER_AC_SELECT_ALL 0x21E // SEL
#define CONSUMER_AC_FIND 0x21F // SEL
#define CONSUMER_AC_FIND_AND_REPLACE 0x220 // SEL
#define CONSUMER_AC_SEARCH 0x221 // SEL
#define CONSUMER_AC_GO_TO 0x222 // SEL
#define CONSUMER_AC_HOME 0x223 // SEL
#define CONSUMER_AC_BACK 0x224 // SEL
#define CONSUMER_AC_FORWARD 0x225 // SEL
#define CONSUMER_AC_STOP 0x226 // SEL
#define CONSUMER_AC_REFRESH 0x227 // SEL
#define CONSUMER_AC_NEXT_LINK 0x229 // SEL
#define CONSUMER_AC_PREVIOUS_LINK 0x228 // SEL
#define CONSUMER_AC_BOOKMARKS 0x22A // SEL
#define CONSUMER_AC_HISTORY 0x22B // SEL
#define CONSUMER_AC_SUBSCRIPTIONS 0x22C // SEL
#define CONSUMER_AC_ZOOM_IN 0x22D // SEL
#define CONSUMER_AC_ZOOM_OUT 0x22E // SEL
#define CONSUMER_AC_ZOOM 0x22F // LC
#define CONSUMER_AC_FULL_SCREEN_VIEW 0x230 // SEL
#define CONSUMER_AC_NORMAL_VIEW 0x231 // SEL
#define CONSUMER_AC_VIEW_TOGGLE 0x232 // SEL
#define CONSUMER_AC_SCROLL_UP 0x233 // SEL
#define CONSUMER_AC_SCROLL_DOWN 0x234 // SEL
#define CONSUMER_AC_SCROLL 0x235 // LC
#define CONSUMER_AC_PAN_LEFT 0x236 // SEL
#define CONSUMER_AC_PAN_RIGHT 0x237 // SEL
#define CONSUMER_AC_PAN 0x238 // LC
#define CONSUMER_AC_NEW_WINDOW 0x239 // SEL
#define CONSUMER_AC_TILE_HORIZONTALLY 0x23A // SEL
#define CONSUMER_AC_TILE_VERTICALLY 0x23B // SEL
#define CONSUMER_AC_FORMAT 0x23C // SEL
#define CONSUMER_AC_EDIT 0x23D // SEL
#define CONSUMER_AC_BOLD 0x23E // SEL
#define CONSUMER_AC_ITALICS 0x23F // SEL
#define CONSUMER_AC_UNDERLINE 0x240 // SEL
#define CONSUMER_AC_STRIKETHROUGH 0x241 // SEL
#define CONSUMER_AC_SUBSCRIPT 0x242 // SEL
#define CONSUMER_AC_SUPERSCRIPT 0x243 // SEL
#define CONSUMER_AC_ALL_CAPS 0x244 // SEL
#define CONSUMER_AC_ROTATE 0x245 // SEL
#define CONSUMER_AC_RESIZE 0x246 // SEL
#define CONSUMER_AC_FLIP_HORIZONTAL 0x247 // SEL
#define CONSUMER_AC_FLIP_VERTICAL 0x248 // SEL
#define CONSUMER_AC_MIRROR_HORIZONTAL 0x249 // SEL
#define CONSUMER_AC_MIRROR_VERTICAL 0x24A // SEL
#define CONSUMER_AC_FONT_SELECT 0x24B // SEL
#define CONSUMER_AC_FONT_COLOR 0x24C // SEL
#define CONSUMER_AC_FONT_SIZE 0x24D // SEL
#define CONSUMER_AC_JUSTIFY_LEFT 0x24E // SEL
#define CONSUMER_AC_JUSTIFY_CENTER_H 0x24F // HSEL
#define CONSUMER_AC_JUSTIFY_RIGHT 0x250 // SEL
#define CONSUMER_AC_JUSTIFY_BLOCK_H 0x251 // HSEL
#define CONSUMER_AC_JUSTIFY_TOP 0x252 // SEL
#define CONSUMER_AC_JUSTIFY_CENTER_V 0x253 // VSEL
#define CONSUMER_AC_JUSTIFY_BOTTOM 0x254 // SEL
#define CONSUMER_AC_JUSTIFY_BLOCK_V 0x255 // VSEL
#define CONSUMER_AC_INDENT_DECREASE 0x256 // SEL
#define CONSUMER_AC_INDENT_INCREASE 0x257 // SEL
#define CONSUMER_AC_NUMBERED_LIST 0x258 // SEL
#define CONSUMER_AC_RESTART_NUMBERING 0x259 // SEL
#define CONSUMER_AC_BULLETED_LIST 0x25A // SEL
#define CONSUMER_AC_PROMOTE 0x25B // SEL
#define CONSUMER_AC_DEMOTE 0x25C // SEL
#define CONSUMER_AC_YES 0x25D // SEL
#define CONSUMER_AC_NO 0x25E // SEL
#define CONSUMER_AC_CANCEL 0x25F // SEL
#define CONSUMER_AC_CATALOG 0x260 // SEL
#define CONSUMER_AC_BUY_CHECKOUT 0x261 // SEL
#define CONSUMER_AC_ADD_TO_CART 0x262 // SEL
#define CONSUMER_AC_EXPAND 0x263 // SEL
#define CONSUMER_AC_EXPAND_ALL 0x264 // SEL
#define CONSUMER_AC_COLLAPSE 0x265 // SEL
#define CONSUMER_AC_COLLAPSE_ALL 0x266 // SEL
#define CONSUMER_AC_PRINT_PREVIEW 0x267 // SEL
#define CONSUMER_AC_PASTE_SPECIAL 0x268 // SEL
#define CONSUMER_AC_INSERT_MODE 0x269 // SEL
#define CONSUMER_AC_DELETE 0x26A // SEL
#define CONSUMER_AC_LOCK 0x26B // SEL
#define CONSUMER_AC_UNLOCK 0x26C // SEL
#define CONSUMER_AC_PROTECT 0x26D // SEL
#define CONSUMER_AC_UNPROTECT 0x26E // SEL
#define CONSUMER_AC_ATTACH_COMMENT 0x26F // SEL
#define CONSUMER_AC_DELETE_COMMENT 0x270 // SEL
#define CONSUMER_AC_VIEW_COMMENT 0x271 // SEL
#define CONSUMER_AC_SELECT_WORD 0x272 // SEL
#define CONSUMER_AC_SELECT_SENTENCE 0x273 // SEL
#define CONSUMER_AC_SELECT_PARAGRAPH 0x274 // SEL
#define CONSUMER_AC_SELECT_COLUMN 0x275 // SEL
#define CONSUMER_AC_SELECT_ROW 0x276 // SEL
#define CONSUMER_AC_SELECT_TABLE 0x277 // SEL
#define CONSUMER_AC_SELECT_OBJECT 0x278 // SEL
#define CONSUMER_AC_REDO_REPEAT 0x279 // SEL
#define CONSUMER_AC_SORT 0x27A // SEL
#define CONSUMER_AC_SORT_ASCENDING 0x27B // SEL
#define CONSUMER_AC_SORT_DESCENDING 0x27C // SEL
#define CONSUMER_AC_FILTER 0x27D // SEL
#define CONSUMER_AC_SET_CLOCK 0x27E // SEL
#define CONSUMER_AC_VIEW_CLOCK 0x27F // SEL
#define CONSUMER_AC_SELECT_TIME_ZONE 0x280 // SEL
#define CONSUMER_AC_EDIT_TIME_ZONES 0x281 // SEL
#define CONSUMER_AC_SET_ALARM 0x282 // SEL
#define CONSUMER_AC_CLEAR_ALARM 0x283 // SEL
#define CONSUMER_AC_SNOOZE_ALARM 0x284 // SEL
#define CONSUMER_AC_RESET_ALARM 0x285 // SEL
#define CONSUMER_AC_SYNCHRONIZE 0x286 // SEL
#define CONSUMER_AC_SEND_RECEIVE 0x287 // SEL
#define CONSUMER_AC_SEND_TO 0x288 // SEL
#define CONSUMER_AC_REPLY 0x289 // SEL
#define CONSUMER_AC_REPLY_ALL 0x28A // SEL
#define CONSUMER_AC_FORWARD_MSG 0x28B // SEL
#define CONSUMER_AC_SEND 0x28C // SEL
#define CONSUMER_AC_ATTACH_FILE 0x28D // SEL
#define CONSUMER_AC_UPLOAD 0x28E // SEL
#define CONSUMER_AC_DOWNLOAD_SAVE_TARGET_AS 0x28F // SEL
#define CONSUMER_AC_SET_BORDERS 0x290 // SEL
#define CONSUMER_AC_INSERT_ROW 0x291 // SEL
#define CONSUMER_AC_INSERT_COLUMN 0x292 // SEL
#define CONSUMER_AC_INSERT_FILE 0x293 // SEL
#define CONSUMER_AC_INSERT_PICTURE 0x294 // SEL
#define CONSUMER_AC_INSERT_OBJECT 0x295 // SEL
#define CONSUMER_AC_INSERT_SYMBOL 0x296 // SEL
#define CONSUMER_AC_SAVE_AND_CLOSE 0x297 // SEL
#define CONSUMER_AC_RENAME 0x298 // SEL
#define CONSUMER_AC_MERGE 0x299 // SEL
#define CONSUMER_AC_SPLIT 0x29A // SEL
#define CONSUMER_AC_DISRIBUTE_HORIZONTALLY 0x29B // SEL
#define CONSUMER_AC_DISTRIBUTE_VERTICALLY 0x29C // SEL
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


#define DIGITIZERS_PAGE              0x0D
#define UNICODE_PAGE                 0x10
#define ALPHANUMERIC_DISPLAY_PAGE    0x14
#define MEDICAL_INSTRUMENT_PAGE      0x40


#endif /* HID_PAGES_H */