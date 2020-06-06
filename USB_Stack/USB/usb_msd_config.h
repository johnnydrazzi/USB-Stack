/**
 * @file usb_msd_config.h
 * @brief <i>Mass Storage Class</i> user settings.
 * @author John Izzard
 * @date 05/06/2020
 * 
 * USB uC - MSD Library.
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

#ifndef USB_MSD_CONFIG
#define USB_MSD_CONFIG

#include "usb_config.h"

#if defined(MSD_SIMPLE_EXAMPLE)
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

#elif defined(MSD_INTERNAL_EXAMPLE)
// External Media Support
//#define USE_EXTERNAL_MEDIA

// Support SCSI Command
#define USE_WRITE_10
//#define USE_PREVENT_ALLOW_MEDIUM_REMOVAL
//#define USE_VERIFY_10

//#define USE_WR_PROTECT
//#define USE_TEST_UNIT_READY
//#define USE_START_STOP_UNIT
//#define USE_READ_CAPACITY   // if not defined use the constant defines for capacity below. 

// CAPACITY
#if defined(__J_PART)
#define BYTES_PER_BLOCK_LE 0x400 // 1024
#define BYTES_PER_BLOCK_BE 0x00040000UL // Big-endian version
#else
#define BYTES_PER_BLOCK_LE 0x200 // 512
#define BYTES_PER_BLOCK_BE 0x00020000UL // Big-endian version
#endif
#if defined(_PIC14E)
#define VOL_CAPACITY_IN_BYTES  0x1000UL // 4KB
#define VOL_CAPACITY_IN_BLOCKS 0x8 // 8

#define LAST_BLOCK_LE 0x7 // 7 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x07000000UL // Big-endian version

#elif defined(_18F14K50) || defined(_18F24K50)
#if _HTC_EDITION_ == 0
#define VOL_CAPACITY_IN_BYTES  0x5800UL // 6KB
#define VOL_CAPACITY_IN_BLOCKS 0x0C // 12

#define LAST_BLOCK_LE 0x0B // 11 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x0B000000UL // Big-endian version
#else
#define VOL_CAPACITY_IN_BYTES  0x6000UL // 8KB
#define VOL_CAPACITY_IN_BLOCKS 0x10 // 16

#define LAST_BLOCK_LE 0x0F // 15 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x0F000000UL // Big-endian version
#endif

#elif defined(_18F25K50) || defined(_18F45K50)
#if _HTC_EDITION_ == 0
#define VOL_CAPACITY_IN_BYTES  0x5800UL // 22KB
#define VOL_CAPACITY_IN_BLOCKS 0x2C // 44

#define LAST_BLOCK_LE 0x2B // 43 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x2B000000UL // Big-endian version
#else
#define VOL_CAPACITY_IN_BYTES  0x6000UL // 24KB
#define VOL_CAPACITY_IN_BLOCKS 0x30 // 48

#define LAST_BLOCK_LE 0x2F // 47 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x2F000000UL // Big-endian version
#endif
#elif defined(_18F27J53) || defined(_18F47J53)
#define VOL_CAPACITY_IN_BYTES  0x01DC00UL // 119KB
#define VOL_CAPACITY_IN_BLOCKS 0x77 // 119

#define LAST_BLOCK_LE 0x76 // 117 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x76000000UL // Big-endian version

#elif defined(_18F26J53) || defined(_18F46J53)
#define VOL_CAPACITY_IN_BYTES  0x00DC00UL // 55KB
#define VOL_CAPACITY_IN_BLOCKS 0x37 // 55

#define LAST_BLOCK_LE 0x36 // 54 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x36000000UL // Big-endian version
#endif

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

#else
// MAKE YOUR OWN
#endif

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