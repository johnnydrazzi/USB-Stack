/**
 * @file ascii_2_key.h
 * @brief Convert ASCII to HID Key values.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * USB uC - HID Examples.
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