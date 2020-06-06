/**
 * @file usb_hid_reports.c
 * @brief <i>Human Interface Device Class</i> user reports.
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

#include "usb_hid_reports.h"

volatile hid_in_report1_t g_hid_in_report1 = {1,0,0};
volatile hid_in_report2_t g_hid_in_report2 = {2,0};

const uint16_t g_hid_in_reports[] =
{
    (uint16_t)&g_hid_in_report1,
    (uint16_t)&g_hid_in_report2
};

const uint8_t g_hid_in_report_size[] =
{
    sizeof(g_hid_in_report1),
    sizeof(g_hid_in_report2)
};

volatile hid_out_report1_t g_hid_out_report1;

const uint16_t g_hid_out_reports[] =
{
    (uint16_t)&g_hid_out_report1
};

const uint8_t g_hid_out_report_size[] =
{
    sizeof(g_hid_out_report1)
};

//volatile hid_feature_report1_t g_hid_feature_report1;
//
//uint16_t g_hid_feature_reports[] =
//{
//    (uint16_t)&g_hid_feature_report1,
//};
//
//uint8_t g_hid_feature_report_size[] =
//{
//    sizeof(g_hid_feature_report1)
//};