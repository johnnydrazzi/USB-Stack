/**
 * @file usb_app.c
 * @brief Contains application specific functions.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - USB Application file (This file is for the CDC Examples).
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
#include "usb.h"
#include "usb_app.h"
#include "usb_cdc.h"


bool usb_service_class_request(void)
{
    return cdc_class_request();
}


bool usb_get_class_descriptor(const uint8_t** descriptor, uint16_t* size)
{
    return false;
}


void usb_app_init(void)
{
    cdc_init();
}


void usb_app_tasks(void)
{
    cdc_tasks();
}


void usb_app_clear_halt(uint8_t bd_table_index, uint8_t ep, uint8_t dir)
{
    g_usb_ep_stat[ep][dir].Halt = 0;
    g_usb_ep_stat[ep][dir].Data_Toggle_Val = 0;
    g_usb_bd_table[bd_table_index].STAT = 0;
}


bool usb_app_set_interface(uint8_t alternate_setting, uint8_t interface)
{
#if NUM_ALT_INTERFACES != 0
    if(g_set_interface.Interface < NUM_INTERFACES) return true;
#else
    if(alternate_setting == 0 && interface == 0)
    {
        CDC_COM_EP_IN_DATA_TOGGLE_VAL = 0;
        return true;
    }
    else if(alternate_setting == 0 && interface == 1)
    {
        CDC_DAT_EP_OUT_DATA_TOGGLE_VAL = 0;
        CDC_DAT_EP_IN_DATA_TOGGLE_VAL = 0;
        return true;
    }
    return false;
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
    return cdc_out_control_tasks();
}