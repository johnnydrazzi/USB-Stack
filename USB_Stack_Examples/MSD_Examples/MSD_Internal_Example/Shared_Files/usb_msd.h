/**
 * @file usb_msd.h
 * @brief <i>Mass Storage Class</i> settings, structures, global variable and function declarations.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - MSD Library.
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
#ifndef USB_MSD_H
#define USB_MSD_H

#include "usb_config.h"


// External Media Support
//#define USE_EXTERNAL_MEDIA

// Support SCSI Command
#define USE_WRITE_10
//#define USE_PREVENT_ALLOW_MEDIUM_REMOVAL
//#define USE_START_STOP_UNIT
//#define USE_VERIFY_10

//#define USE_WR_PROTECT
//#define USE_TEST_UNIT_READY
//#define USE_START_STOP_UNIT
//#define USE_READ_CAPACITY   // if not defined use the constant defines for capacity below. 

// RAM Setting
#if defined(_PIC14E) || defined(_18F14K50)
#define MSD_LIMITED_RAM // PIC18F145X and PIC18F14K50 need this settings as RAM is tight on these parts.
#warning "Please note: The msd library is using the MSD_LIMITED_RAM setting, as this part has a small amount of RAM. \
Use R_W_10_Vars.LBA in combination with ByteOfSect to find locations in the sector."
#endif
#define MSD_LIMITED_RAM // **HIGHLY RECOMMEND TRYING THIS SITTING.
                        // Will reduce ROM and RAM size and speed up code, 
                        // at the cost of slightly more complicated tracking 
                        // of Sector locations using R_W_10_Vars.LBA in combination 
                        // with ByteOfSect.

// MSD Endpoint HAL
#define MSD_EP EP1
#define MSD_EP_SIZE EP1_SIZE

#ifdef _PIC14E
#warning "MSD EP Buffer addresses need to be manually set for PIC16 devices."
#endif

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_1_15
#define MSD_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*2))
#elif PINGPONG_MODE == PINGPONG_0_OUT
#define MSD_EP_BUFFERS_STARTING_ADDR  (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*3))
#else
#define MSD_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*4))
#endif

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_0_OUT
#ifdef _PIC14E
#define MSD_EP_OUT_BUFFER_BASE_ADDR 0x2050
#define MSD_EP_IN_BUFFER_BASE_ADDR  0x20A0
#else
#define MSD_EP_OUT_BUFFER_BASE_ADDR  MSD_EP_BUFFERS_STARTING_ADDR
#define MSD_EP_IN_BUFFER_BASE_ADDR   (MSD_EP_BUFFERS_STARTING_ADDR + EP1_SIZE)
#endif
uint8_t MSD_EP_OUT[MSD_EP_SIZE] @MSD_EP_OUT_BUFFER_BASE_ADDR;
uint8_t MSD_EP_IN[MSD_EP_SIZE]  @MSD_EP_IN_BUFFER_BASE_ADDR;

#else // PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
#ifdef _PIC14E
#define MSD_EP_OUT_EVEN_BUFFER_BASE_ADDR 0x2050
#define MSD_EP_OUT_ODD_BUFFER_BASE_ADDR  0x20A0
#define MSD_EP_IN_EVEN_BUFFER_BASE_ADDR  0x20F0
#define MSD_EP_IN_ODD_BUFFER_BASE_ADDR   0x2140
#else
#define MSD_EP_OUT_EVEN_BUFFER_BASE_ADDR  MSD_EP_BUFFERS_STARTING_ADDR
#define MSD_EP_OUT_ODD_BUFFER_BASE_ADDR   (MSD_EP_BUFFERS_STARTING_ADDR + EP1_SIZE)
#define MSD_EP_IN_EVEN_BUFFER_BASE_ADDR   (MSD_EP_BUFFERS_STARTING_ADDR + (EP1_SIZE*2))
#define MSD_EP_IN_ODD_BUFFER_BASE_ADDR    (MSD_EP_BUFFERS_STARTING_ADDR + (EP1_SIZE*3))
#endif
uint8_t MSD_EP_OUT_EVEN[MSD_EP_SIZE]    @MSD_EP_OUT_EVEN_BUFFER_BASE_ADDR;
uint8_t MSD_EP_OUT_ODD[MSD_EP_SIZE]     @MSD_EP_OUT_ODD_BUFFER_BASE_ADDR;
uint8_t MSD_EP_IN_EVEN[MSD_EP_SIZE]     @MSD_EP_IN_EVEN_BUFFER_BASE_ADDR;
uint8_t MSD_EP_IN_ODD[MSD_EP_SIZE]      @MSD_EP_IN_ODD_BUFFER_BASE_ADDR;
#endif

#define MSD_BD_OUT BD1_OUT
#define MSD_BD_OUT_EVEN BD1_OUT_EVEN
#define MSD_BD_OUT_ODD BD1_OUT_ODD
#define MSD_BD_IN BD1_IN
#define MSD_BD_IN_EVEN BD1_IN_EVEN
#define MSD_BD_IN_ODD BD1_IN_ODD

#define MSD_UEPbits UEP1bits

// CAPACITY
#if defined(__J_PART)
#define BYTES_PER_BLOCK_LE 0x400 // 1024
#define BYTES_PER_BLOCK_BE 0x00040000UL // Big-endian version
#else
#define BYTES_PER_BLOCK_LE 0x200 // 512
#define BYTES_PER_BLOCK_BE 0x00020000UL // Big-endian version
#endif
#if defined(_PIC14E)
#define VOL_CAPACITY_IN_BYTES 0x1000UL // 4KB
#define VOL_CAPACITY_IN_BLOCKS 0x8 // 8

#define LAST_BLOCK_LE 0x7 // 7 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x07000000UL // Big-endian version

#elif defined(_18F14K50) || defined(_18F24K50)
#if _HTC_EDITION_ == 0
#define VOL_CAPACITY_IN_BYTES 0x5800UL // 6KB
#define VOL_CAPACITY_IN_BLOCKS 0x0C // 12

#define LAST_BLOCK_LE 0x0B // 11 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x0B000000UL // Big-endian version
#else
#define VOL_CAPACITY_IN_BYTES 0x6000UL // 8KB
#define VOL_CAPACITY_IN_BLOCKS 0x10 // 16

#define LAST_BLOCK_LE 0x0F // 15 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x0F000000UL // Big-endian version
#endif

#elif defined(_18F25K50) || defined(_18F45K50)
#if _HTC_EDITION_ == 0
#define VOL_CAPACITY_IN_BYTES 0x5800UL // 22KB
#define VOL_CAPACITY_IN_BLOCKS 0x2C // 44

#define LAST_BLOCK_LE 0x2B // 43 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x2B000000UL // Big-endian version
#else
#define VOL_CAPACITY_IN_BYTES 0x6000UL // 24KB
#define VOL_CAPACITY_IN_BLOCKS 0x30 // 48

#define LAST_BLOCK_LE 0x2F // 47 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x2F000000UL // Big-endian version
#endif
#elif defined(_18F27J53) || defined(_18F47J53)
#define VOL_CAPACITY_IN_BYTES 0x01DC00UL // 119KB
#define VOL_CAPACITY_IN_BLOCKS 0x77 // 119

#define LAST_BLOCK_LE 0x76 // 117 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x76000000UL // Big-endian version

#elif defined(_18F26J53) || defined(_18F46J53)
#define VOL_CAPACITY_IN_BYTES 0x00DC00UL // 55KB
#define VOL_CAPACITY_IN_BLOCKS 0x37 // 55

#define LAST_BLOCK_LE 0x36 // 54 (VOL_CAPACITY_IN_BLOCKS - 1)
#define LAST_BLOCK_BE 0x36000000UL // Big-endian version
#endif

// The Thirteen Error Cases
#define CASE_1  0x0001
#define CASE_2  0x0002
#define CASE_3  0x0004
#define CASE_4  0x0008
#define CASE_5  0x0010
#define CASE_6  0x0020
#define CASE_7  0x0040
#define CASE_8  0x0080
#define CASE_9  0x0100
#define CASE_10 0x0200
#define CASE_11 0x0400
#define CASE_12 0x0800
#define CASE_13 0x1000

// MSD Subclass Codes
#define SCSI_CMD_N_SUPPORT 0x00
#define RBC                0x01
#define MMC_5              0x02
#define QIC_157            0x03
#define UFI                0x04
#define SFF_8070i          0x05
#define SCSI_TRANSPARENT   0x06
#define LSD_FS             0x07
#define IEEE_1667          0x08

// MSD Protocol Codes
#define CBI_W_COMPL_INT    0x00
#define CBI_N_COMPL_INT    0x01
#define BBB                0x50
#define UAS                0x62

// Class Specific Descriptor Codes
#define PIPE_USAGE_DESC    0x24

// dCBWSignature/dCSWSignature values
#define CBW_SIG 0x43425355
#define CSW_SIG 0x53425355

// MSD STATES
#define MSD_CBW           0
#define MSD_NO_DATA_STAGE 1
#define MSD_DATA_SENT     2
#define MSD_CSW           3
#define MSD_READ_DATA     4
#define MSD_WRITE_DATA    5
#define MSD_WAIT_ILLEGAL  6
#define MSD_WAIT_IVALID   7
#define MSD_READ_FINISHED 8

// MSD Request Codes
#define ADSC 0
#define GET_REQUESTS 0xFC
#define PUT_REQUESTS 0xFD
#define GET_MAX_LUN  0xFE
#define BOMSR        0xFF

#include <stdint.h>
#include <stdbool.h>
#include "usb_hal.h"

bool MSD_ClassRequest(void);
void MSD_Init(void);
void MSD_AddTask(void);
void MSD_Tasks(void);
void MSD_ClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir);
void MSD_ClearEndpointToggle(void);
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
void MSD_Arm_EP_OUT(uint8_t BDT_Index);
void MSD_Arm_EP_IN(uint8_t BDT_Index, uint16_t cnt);
#else
void MSD_Arm_EP_OUT(void);
void MSD_Arm_EP_IN(uint16_t cnt);
#endif

bool MSD_CheckForMedia(void);
uint8_t MSD_TestUnitReady(void);
uint8_t MSD_StartStopUnit(void);
void MSD_ReadCapacity(void);
void MSD_RxSector(void);
void MSD_TxSector(void);
bool MSD_WrProtect(void);

#define MSD_TRANSACTION_DIR MSD_Tasks_Buffer.Task_STAT[MSD_TgetIndex].DIR
#define MSD_PINGPONG_PARITY MSD_Tasks_Buffer.Task_STAT[MSD_TgetIndex].PPBI

#define MSD_EP_OUT_LAST_PPB        EPn_Stat.Array[MSD_EP][OUT].Last_PPB
#define MSD_EP_IN_LAST_PPB         EPn_Stat.Array[MSD_EP][IN].Last_PPB
#define MSD_EP_OUT_DATA_TOGGLE_VAL EPn_Stat.Array[MSD_EP][OUT].Data_Toggle_Val
#define MSD_EP_IN_DATA_TOGGLE_VAL  EPn_Stat.Array[MSD_EP][IN].Data_Toggle_Val

uint16_t ByteOfSect;
#ifndef MSD_LIMITED_RAM
uint8_t MSD_SectData[512];
#endif

#if defined(_PIC14E)
#define CBW_DATA_ADDR SETUP_DATA_ADDR - 31
#else
#define CBW_DATA_ADDR SETUP_DATA_ADDR + 8
#endif

///** Generic Command Wrapper Structure */
//struct{
//    uint32_t dCWSignature;
//    uint32_t dCWTag;
//}Generic_CW_Data @EP1_OUT_BUFFER_BASE_ADDR;

/** Command Block Wrapper Structure */
union{
    uint8_t BYTES[31];
    struct{
        uint32_t dCBWSignature;
        uint32_t dCBWTag;
        uint32_t dCBWDataTransferLength;
        union{
            uint8_t  bmCBWFlags;
            struct {
                unsigned :7;
                unsigned Direction:1;
            };
        };
        uint8_t  bCBWLUN;
        uint8_t  bCBWCBLength;
        uint8_t  CBWCB0[16];
    }; 
}CBW_Data @CBW_DATA_ADDR;

/** Command Status Wrapper Structure */
union{
    uint8_t BYTES[13];
    struct{
        uint32_t dCSWSignature;
        uint32_t dCSWTag;
        uint32_t dCSWDataResidue;
        uint8_t  bCSWStatus;
    };
}CSW_Data;

/// Status Values
#define COMMAND_PASSED 0
#define COMMAND_FAILED 1
#define PHASE_ERROR    2

/** READ_10 Variables Structure */
struct{
    union{
        uint8_t START_LBA_BYTES[4];
        uint32_t START_LBA;
    };
    uint32_t LBA;
    union{
        uint8_t TF_LEN_BYTES[2];
        uint16_t TF_LEN;
    };
    uint32_t TF_LEN_IN_BYTES;
    uint32_t CBW_TF_LEN;
}R_W_10_Vars;

/** Stores the amount of bytes to be return in responses. */
union{
    uint16_t val;
    struct{
        uint8_t LB;
        uint8_t HB;
    };
}BytesToTransfer;

#endif /* USB_MSD_H */