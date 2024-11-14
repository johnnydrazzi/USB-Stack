/**
 * @file usb_app.c
 * @brief Contains application specific functions.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - USB Application file (This file is for the HID Mouse Example).
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
 
#include <stdbool.h>
#include "../../../../USB/usb.h"
#include "../../../../USB/usb_app.h"
#include "../../../../USB/usb_hid.h"


bool usb_service_class_request(void)
{
    return hid_class_request();
}


bool usb_get_class_descriptor(const uint8_t** descriptor, uint16_t* size)
{
    return hid_get_class_descriptor(descriptor, size);
}


void usb_app_init(void)
{
    hid_init();
}


void usb_app_tasks(void)
{
    if(TRANSACTION_EP == HID_EP) hid_tasks();
}


void usb_app_clear_halt(uint8_t bd_table_index, uint8_t ep, uint8_t dir)
{
    hid_clear_halt(bd_table_index, ep, dir);
}


bool usb_app_set_interface(uint8_t alternate_setting, uint8_t interface)
{
#if NUM_ALT_INTERFACES != 0
    if(g_set_interface.Interface < NUM_INTERFACES) return true;
#else
    if(alternate_setting != 0 || interface != 0) return false;
    
    hid_clear_ep_toggle();
    return true;
#endif
}


bool usb_app_get_interface(uint8_t* alternate_setting_result, uint8_t interface)
{
#if NUM_ALT_INTERFACES != 0
    if(g_get_interface.Interface < NUM_INTERFACES) return true;
#endif
    return false;
}


bool usb_out_control_finished(void)
{
    return false;
}