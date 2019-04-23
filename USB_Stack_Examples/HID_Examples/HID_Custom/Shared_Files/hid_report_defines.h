/**
 * @file hid_report_defines.h
 * @brief <i>Human Interface Device Class</i> Report Defines.
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
#ifndef HID_REPORT_DEFINES_H
#define HID_REPORT_DEFINES_H

#define ZERO_BYTES 0
#define ONE_BYTES  1
#define TWO_BYTES  2
#define FOUR_BYTES 3

// Main Items
#define Input(x)         0x81, x
#define Output(x)        0x91, x
#define Feature(x)       0xB1, x
#define Collection(x)    0xA1, x
#define End_Collection() 0xC0

// Input/Output/Feature Items
#define DATA                0x00
#define CONSTANT            0x01
#define ARRAY               0x00
#define VARIABLE            0x02
#define ABSOLUTE            0x00
#define RELATIVE            0x04
#define NO_WRAP             0x00
#define WRAP                0x08
#define LINEAR              0x00
#define NONLINEAR           0x10
#define PREFERRED_STATE     0x00
#define NO_PREFERRED        0x20
#define NO_NULL_POSITION    0x00
#define NULL_STATE          0x40
#define BIT_FIELD           0x00
#define BUFFERED_BYTES      0x80

// Collection Items
#define PHYSICAL     0x00
#define APPLICATION  0x01
#define LOGICAL      0x02
#define REPORT       0x03
#define NAMED_ARRAY  0x04
#define USAGE_SWITCH 0x05
#define USAGE_MOD    0x06

// Global Items
#define Usage_Page(x)        0x05, x
#define Usage_Page2(x)       0x06, x & 0xFF, (x>>8) & 0xFF
#define Usage_Page4(x)       0x07, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Logic_Minimum(x)     0x15, x
#define Logic_Minimum2(x)    0x16, x & 0xFF, (x>>8) & 0xFF
#define Logic_Minimum4(x)    0x17, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Logic_Maximum(x)     0x25, x
#define Logic_Maximum2(x)    0x26, x & 0xFF, (x>>8) & 0xFF
#define Logic_Maximum4(x)    0x27, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Physical_Minimum(x)  0x35, x
#define Physical_Minimum2(x) 0x36, x & 0xFF, (x>>8) & 0xFF
#define Physical_Minimum4(x) 0x37, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Physical_Maximum(x)  0x45, x
#define Physical_Maximum2(x) 0x46, x & 0xFF, (x>>8) & 0xFF
#define Physical_Maximum4(x) 0x47, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Unit_Exponent(x)     0x55, x
#define Unit(x)              0x65, x
#define Report_Size(x)       0x75, x
#define Report_ID(x)         0x85, x
#define Report_Count(x)      0x95, x
#define Push(x)              0xA4|ONE_BYTES, x
#define Pop(x)               0xB4|ONE_BYTES, x

// Local Items
#define Usage(x)              0x09, x
#define Usage2(x)             0x0A, x & 0xFF, (x>>8) & 0xFF
#define Usage4(x)             0x0B, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Usage_Minimum(x)      0x19, x
#define Usage_Minimum2(x)     0x1A, x & 0xFF, (x>>8) & 0xFF
#define Usage_Minimum4(x)     0x1B, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Usage_Maximum(x)      0x29, x
#define Usage_Maximum2(x)     0x2A, x & 0xFF, (x>>8) & 0xFF
#define Usage_Maximum4(x)     0x2B, x & 0xFF, (x>>8) & 0xFF, (x>>16) & 0xFF, (x>>24) & 0xFF
#define Designator_Index(x)   0x38|ONE_BYTES, x
#define Designator_Minimum(x) 0x48|ONE_BYTES, x
#define Designator_Maximum(x) 0x58|ONE_BYTES, x
#define String_Index(x)       0x78|ONE_BYTES, x
#define String_Minimum(x)     0x88|ONE_BYTES, x
#define String_Maximum(x)     0x98|ONE_BYTES, x
#define Delimeter(x)          0xA8|ONE_BYTES, x

#endif /* HID_REPORT_DEFINES_H */