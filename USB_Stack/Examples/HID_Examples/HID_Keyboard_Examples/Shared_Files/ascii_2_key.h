/**
 * @file ascii_2_key.h
 * @brief Convert ASCII to HID Key values.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - HID Examples.
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
 
#ifndef ASCII_2_KEY_H
#define ASCII_2_KEY_H

#include <stdint.h>
#include "usb_hid.h"
#include "usb_hid_pages.h"

// Choose a End of Line Character (\n or \r) used to convert to enter/return.
// Windows uses \r\n so use either \n or \r.
// Unix uses \n.
// Mac uses \r.
#define ELC '\r'

typedef struct
{
    uint8_t Modifier;
    uint8_t KeyCode;
}Key_Result_t;

extern Key_Result_t g_key_result;

void ascii_2_key(uint8_t val);

#endif /* ASCII_2_KEY_H */