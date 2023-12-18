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
#include "usb_hid_config.h"

typedef struct
{
    uint8_t array[HID_EP_SIZE];
}hid_in_report1_t;

typedef struct
{
    uint8_t array[HID_EP_SIZE];
}hid_out_report1_t;

typedef struct
{

}hid_feature_report1_t;

extern volatile hid_in_report1_t  g_hid_in_report1;
extern const    uint16_t          g_hid_in_reports[];
extern const    uint8_t           g_hid_in_report_size[];

extern volatile hid_out_report1_t g_hid_out_report1;
extern const    uint16_t          g_hid_out_reports[];
extern const    uint8_t           g_hid_out_report_size[];

#endif /* USB_HID_REPORTS_H */