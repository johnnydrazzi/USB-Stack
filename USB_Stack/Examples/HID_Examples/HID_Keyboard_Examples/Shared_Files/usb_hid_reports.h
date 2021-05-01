/**
 * @file usb_hid_reports.h
 * @brief <i>Human Interface Device Class</i> user report types.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * USB uC - HID Library.
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

#ifndef USB_HID_REPORTS_H
#define USB_HID_REPORTS_H

#include <stdint.h>

typedef struct
{
    uint8_t Report_ID;
    union
    {
        uint8_t Modifiers;
        struct
        {
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
}hid_in_report1_t;

typedef struct
{
    uint8_t Report_ID;
    union
    {
        uint8_t Consumer_Byte;
        struct
        {
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
}hid_in_report2_t;

typedef struct
{
    uint8_t Report_ID;
    struct{
        unsigned NUM_LOCK    :1;
        unsigned CAPS_LOCK   :1;
        unsigned SCROLL_LOCK :1;
        unsigned COMPOSE     :1;
        unsigned KANA        :1;
        unsigned             :3;
    };
}hid_out_report1_t;

typedef struct
{

}hid_feature_report1_t;

extern volatile hid_in_report1_t  g_hid_in_report1;
extern volatile hid_in_report2_t  g_hid_in_report2;
extern const    uint16_t          g_hid_in_reports[];
extern const    uint8_t           g_hid_in_report_size[];

extern volatile hid_out_report1_t g_hid_out_report1;
extern const    uint16_t          g_hid_out_reports[];
extern const    uint8_t           g_hid_out_report_size[];

#endif /* USB_HID_REPORTS_H */