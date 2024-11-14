/**
 * @file usb_msd_config.h
 * @brief <i>Mass Storage Class</i> user settings.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - MSD Library.
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

#ifndef USB_MSD_CONFIG
#define USB_MSD_CONFIG

#include "usb_config.h"

// External Media Support
//#define USE_EXTERNAL_MEDIA

// Support SCSI Command
//#define USE_WRITE_10
//#define USE_PREVENT_ALLOW_MEDIUM_REMOVAL
//#define USE_VERIFY_10

//#define USE_WR_PROTECT
//#define USE_TEST_UNIT_READY
//#define USE_START_STOP_UNIT
//#define USE_READ_CAPACITY   // if not defined use the constant defines for capacity below. 

// CAPACITY
#define BYTES_PER_BLOCK_LE 0x200 // 512
#define BYTES_PER_BLOCK_BE 0x00020000UL // Big-endian version

#define VOL_CAPACITY_IN_BYTES 0x20000UL // 128KB
#define VOL_CAPACITY_IN_BLOCKS 0x100 // 256

#define LAST_BLOCK_LE 0xFF // 255 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0xFF000000UL // Big-endian version

// MSD Endpoint HAL
#define MSD_EP EP1
#define MSD_EP_SIZE EP1_SIZE

// MSD Buffer Decriptor HAL
#define MSD_BD_OUT         BD1_OUT
#define MSD_BD_OUT_EVEN    BD1_OUT_EVEN
#define MSD_BD_OUT_ODD     BD1_OUT_ODD
#define MSD_BD_IN          BD1_IN
#define MSD_BD_IN_EVEN     BD1_IN_EVEN
#define MSD_BD_IN_ODD      BD1_IN_ODD

// MSD UEP1bits
#define MSD_UEPbits UEP1bits

// RAM Setting
#if defined(_PIC14E) || defined(_18F14K50)
#define MSD_LIMITED_RAM // PIC18F145X and PIC18F14K50 need this settings as RAM is tight on these parts.
#warning "Please note: The msd library is using the MSD_LIMITED_RAM setting, as this part has a small amount of RAM. \
Use R_W_10_Vars.LBA in combination with g_msd_byte_of_sect to find locations in the sector."
#endif
#define MSD_LIMITED_RAM // **HIGHLY RECOMMEND TRYING THIS SETTING.
                        // Will reduce ROM and RAM size and speed up code, 
                        // at the cost of slightly more complicated tracking 
                        // of Sector locations using g_msd_rw_10_vars.LBA in combination 
                        // with g_msd_byte_of_sect.

#endif