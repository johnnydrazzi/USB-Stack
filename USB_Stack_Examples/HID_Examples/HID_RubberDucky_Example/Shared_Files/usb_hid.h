/**
 * @file usb_hid.h
 * @brief <i>Human Interface Device Class</i> settings, structures, global variable and function declarations.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - HID Library (For Keyboard Examples).
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
#ifndef USB_HID_H
#define USB_HID_H

#include "usb_config.h"
#include "usb_hal.h"

// HID Endpoint HAL
#define HID_EP EP1
#define HID_EP_SIZE EP1_SIZE

#ifdef _PIC14E
#warning "HID EP Buffer addresses need to be manually set for PIC16 devices."
#endif

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_1_15
#define HID_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*2))
#elif PINGPONG_MODE == PINGPONG_0_OUT
#define HID_EP_BUFFERS_STARTING_ADDR  (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*3))
#else
#define HID_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*4))
#endif

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_0_OUT
#ifdef _PIC14E
#define HID_EP_OUT_BUFFER_BASE_ADDR 0x2050
#define HID_EP_IN_BUFFER_BASE_ADDR  0x20A0
#else
#define HID_EP_OUT_BUFFER_BASE_ADDR  HID_EP_BUFFERS_STARTING_ADDR
#define HID_EP_IN_BUFFER_BASE_ADDR   (HID_EP_BUFFERS_STARTING_ADDR + EP1_SIZE)
#endif
uint8_t HID_EP_OUT[HID_EP_SIZE] @HID_EP_OUT_BUFFER_BASE_ADDR;
uint8_t HID_EP_IN[HID_EP_SIZE]  @HID_EP_IN_BUFFER_BASE_ADDR;

#else // PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
#ifdef _PIC14E
#define HID_EP_OUT_EVEN_BUFFER_BASE_ADDR 0x2050
#define HID_EP_OUT_ODD_BUFFER_BASE_ADDR  0x20A0
#define HID_EP_IN_EVEN_BUFFER_BASE_ADDR  0x20F0
#define HID_EP_IN_ODD_BUFFER_BASE_ADDR   0x2140
#else
#define HID_EP_OUT_EVEN_BUFFER_BASE_ADDR  HID_EP_BUFFERS_STARTING_ADDR
#define HID_EP_OUT_ODD_BUFFER_BASE_ADDR   (HID_EP_BUFFERS_STARTING_ADDR + EP1_SIZE)
#define HID_EP_IN_EVEN_BUFFER_BASE_ADDR   (HID_EP_BUFFERS_STARTING_ADDR + (EP1_SIZE*2))
#define HID_EP_IN_ODD_BUFFER_BASE_ADDR    (HID_EP_BUFFERS_STARTING_ADDR + (EP1_SIZE*3))
#endif
uint8_t HID_EP_OUT_EVEN[HID_EP_SIZE]    @HID_EP_OUT_EVEN_BUFFER_BASE_ADDR;
uint8_t HID_EP_OUT_ODD[HID_EP_SIZE]     @HID_EP_OUT_ODD_BUFFER_BASE_ADDR;
uint8_t HID_EP_IN_EVEN[HID_EP_SIZE]     @HID_EP_IN_EVEN_BUFFER_BASE_ADDR;
uint8_t HID_EP_IN_ODD[HID_EP_SIZE]      @HID_EP_IN_ODD_BUFFER_BASE_ADDR;
#endif

#define HID_BD_OUT BD1_OUT
#define HID_BD_OUT_EVEN BD1_OUT_EVEN
#define HID_BD_OUT_ODD BD1_OUT_ODD
#define HID_BD_IN BD1_IN
#define HID_BD_IN_EVEN BD1_IN_EVEN
#define HID_BD_IN_ODD BD1_IN_ODD

#define HID_UEPbits UEP1bits


/* DEFINES/STRUCTS FOR USB DESCRIPTORS */
// HID Subclass Codes
#define HID_BOOT 1

// HID Protocol Codes
#define HID_KEYBOARD 1
#define HID_MOUSE    2

#define HID_DESC          0x21
#define HID_REPORT_DESC   0x22
#define HID_PHYSICAL_DESC 0x23

#include <stdint.h>
#include <stdbool.h>

struct HID_Descriptor_Type{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdHID;
    uint8_t  bCountryCode;
    uint8_t  bNumDescriptors;
    uint8_t  bClassDescriptorType;
    uint16_t wDescriptorLength;
};


/* GLOBAL FUNCTIONS/VARS */
bool HID_ClassRequest(void);
bool HID_GetClassDescriptor(void);
void HID_Init(void);
void HID_Tasks(void);
void HID_ClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir);
void HID_SetSentReportFlag(void);
void HID_ClearEndpointToggle(void);
void HID_SendReport(uint8_t ReportNum);
void HID_ServiceSOF(void);
void HID_Out(void);
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
void HID_Arm_EP_OUT(uint8_t BDT_Index);
void HID_Arm_EP_IN(uint8_t BDT_Index, uint16_t cnt);
#else
void HID_Arm_EP_OUT(void);
void HID_Arm_EP_IN(uint16_t cnt);
#endif

#define HID_EP_OUT_LAST_PPB        EPn_Stat.Array[HID_EP][OUT].Last_PPB
#define HID_EP_IN_LAST_PPB         EPn_Stat.Array[HID_EP][IN].Last_PPB
#define HID_EP_OUT_DATA_TOGGLE_VAL EPn_Stat.Array[HID_EP][OUT].Data_Toggle_Val
#define HID_EP_IN_DATA_TOGGLE_VAL  EPn_Stat.Array[HID_EP][IN].Data_Toggle_Val

volatile bool ReportSent = true;
volatile uint8_t ReportNumSent;


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
#define IN_REPORT_EP EP1
#define IN_REPORT_BDT BD1_IN
#define IN_REPORT_EP_ADDR EP1_IN_BUFFER_BASE_ADDR

// Idle_Settings
#define DEFAULT_IDLE 500 // in mS


/* REPORT DESCRIPTOR */
#include "hid_pages.h"
#include "hid_report_defines.h"

// Report Descriptor (Keyboard)
const uint8_t HID_Report_Descriptor[] = {
    Usage_Page(GENERIC_DESKTOP_PAGE),
    Usage(KEYBOARD),
    Collection(APPLICATION),
        Report_ID(1),
    
        Usage_Page(LED_PAGE),
        Usage_Minimum(LED_NUM_LOCK),
        Usage_Maximum(LED_KANA),
        Logic_Minimum(0),
        Logic_Maximum(1),
        Report_Size(1),
        Report_Count(5),
        Output(DATA|VARIABLE|ABSOLUTE),
        Logic_Minimum(0),
        Logic_Maximum(0),
        Report_Count(1),
        Report_Size(3),
        Output(CONSTANT),
    
        Usage_Page(KEYBOARD_KEYPAD_PAGE),
        Usage_Minimum(KEY_LEFTCTRL),
        Usage_Maximum(KEY_RIGHTMETA),
        Logic_Minimum(0),
        Logic_Maximum(1),
        Report_Size(1),
        Report_Count(8),
        Input(DATA|VARIABLE|ABSOLUTE),
    
        Usage_Minimum(0),
        Usage_Maximum(101),
        Logic_Minimum(0),
        Logic_Maximum(101),      
        Report_Size(8),
        Report_Count(1),
        Input(DATA|ARRAY),
    End_Collection(),
    
    Usage_Page(CONSUMER_PAGE),
    Usage(CONSUMER_CONSUMER_CONTROL),
    Collection(APPLICATION),
        Report_ID(2),
        Usage_Page(CONSUMER_PAGE),
        Usage(CONSUMER_SCAN_NEXT_TRACK),
        Usage(CONSUMER_SCAN_PREVIOUS_TRACK),
        Usage(CONSUMER_STOP),
        Usage(CONSUMER_EJECT),
        Usage(CONSUMER_PLAY_PAUSE),
        Usage(CONSUMER_MUTE),
        Usage(CONSUMER_VOLUME_INCREMENT),
        Usage(CONSUMER_VOLUME_DECREMENT),
        Logic_Minimum(0),
        Logic_Maximum(1),
        Report_Size(1),
        Report_Count(8),
        Input(DATA|VARIABLE|ABSOLUTE),
    End_Collection()
};


/* REPORT STRUCTURES/VARS/DEFINES */
#define HID_USE_REPORT_IDS 0
#define HID_NUM_REPORT_IDS 2

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

#define HID_NUM_IN_REPORTS  2
#define HID_IN_REPORT1_SIZE 3
volatile union{
    uint8_t Array[HID_IN_REPORT1_SIZE];
    struct{
        uint8_t Report_ID;
        union{
            uint8_t Modifiers;
            struct{
                unsigned LEFT_CTRL   :1;
                unsigned LEFT_SHIFT  :1;
                unsigned LEFT_ALT    :1;
                unsigned LEFT_GUI    :1;
                unsigned RIGHT_CTRL  :1;
                unsigned RIGHT_SHIFT :1;
                unsigned RIGHT_ALT   :1;
                unsigned RIGHT_GUI   :1;
            };
        };
        //uint8_t Reserved;
        //uint8_t Keycode[6];
        uint8_t Keycode;
    }data;
}In_Report1 = {1,0,0};

#define HID_IN_REPORT2_SIZE 2
volatile union{
    uint8_t Array[HID_IN_REPORT2_SIZE];
    struct{
        uint8_t Report_ID;
        union{
            uint8_t Consumer_Byte;
            struct{
                unsigned SCAN_NEXT_TRACK     :1;
                unsigned SCAN_PREVIOUS_TRACK :1;
                unsigned STOP                :1;
                unsigned EJECT               :1;
                unsigned PLAY_PAUSE          :1;
                unsigned MUTE                :1;
                unsigned VOLUME_INCREMENT    :1;
                unsigned VOLUME_DECREMENT    :1;
            };
        };
    }data;
}In_Report2 = {2,0};

volatile bool SentReportFlag[HID_NUM_IN_REPORTS] = {true};

const uint16_t In_Report_Ptr[] = {
    (uint16_t)&In_Report1,
    (uint16_t)&In_Report2
};

const uint8_t In_Report_Size[] = {
    HID_IN_REPORT1_SIZE,
    HID_IN_REPORT2_SIZE
};

struct In_Report_Setting_Type{
    uint8_t  Idle_Duration_4ms;
    uint16_t Idle_Duration_1ms;
    uint16_t Idle_Count;
	bool Idle_Count_Overflow;
};

volatile struct In_Report_Setting_Type In_Report_Settings[HID_NUM_IN_REPORTS];

#define HID_NUM_OUT_REPORTS 1
#define HID_OUT_REPORT1_SIZE 2
volatile struct{
    uint8_t Report_ID;
    struct{
        unsigned NUM_LOCK    :1;
        unsigned CAPS_LOCK   :1;
        unsigned SCROLL_LOCK :1;
        unsigned COMPOSE     :1;
        unsigned KANA        :1;
        unsigned             :3;
    };
}Out_Report1;

uint16_t Out_Report_Ptr[] = {
    (uint16_t)&Out_Report1
};

uint8_t Out_Report_Size[] = {
    HID_OUT_REPORT1_SIZE
};

#define HID_NUM_FEATURE_REPORTS 0
#define HID_FEATURE_REPORT1_SIZE 0
//volatile struct{
//    
//}Feature_Report1;
//
//uint16_t Feature_Report_Ptr[] = {
//    (uint16_t)&Feature_Report1,
//};
//
//uint8_t Feature_Report_Size[] = {
//    HID_FEATURE_REPORT1_SIZE
//};

#endif /* USB_HID_H */