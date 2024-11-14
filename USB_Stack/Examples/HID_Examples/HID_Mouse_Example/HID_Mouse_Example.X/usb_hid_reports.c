/**
 * @file usb_hid_reports.c
 * @brief <i>Human Interface Device Class</i> user reports.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - HID Library.
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

#include "usb_hid_reports.h"

volatile hid_in_report1_t g_hid_in_report1 = {0};

const uint16_t g_hid_in_reports[] =
{
    (uint16_t)&g_hid_in_report1,
};

const uint8_t g_hid_in_report_size[] =
{
    sizeof(g_hid_in_report1)
};

//volatile hid_out_report1_t g_hid_out_report1;
//
//const uint16_t g_hid_out_reports[] =
//{
//    (uint16_t)&g_hid_out_report1
//};
//
//const uint8_t g_hid_out_report_size[] =
//{
//    sizeof(g_hid_out_report1)
//};

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