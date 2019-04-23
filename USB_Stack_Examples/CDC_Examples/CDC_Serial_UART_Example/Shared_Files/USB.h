/**
 * @file USB.h
 * @brief Contains definitions used by the core USB stack and declaration of global functions and variables.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - USB Stack.
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

#ifndef USB_H
#define USB_H

/*********************** STATE Definitions *****************************/
#define Detached   0
#define Attached   1
#define Powered    2
#define Default    3
#define Address    4
#define Suspended  5
#define Configured 6
/**********************************************************************/

/*********************** Control Transfer Stages *****************************/
#define SETUP_STAGE      0
#define DATA_IN_STAGE    1
#define DATA_OUT_STAGE   2
#define STATUS_IN_STAGE  3
#define STATUS_OUT_STAGE 4
/*****************************************************************************/

/*************** PIDs ******************/
// Token PIDs
#define PID_OUT_TOKEN    0b0001
#define PID_IN_TOKEN     0b1001
#define PID_SOF_TOKEN    0b0101
#define PID_SETUP_TOKEN  0b1101

// Data PIDs
#define PID_DATA0        0b0011
#define PID_DATA1        0b1011
#define PID_DATA2        0b0111
#define PID_MDATA        0b1111

// Handshake PIDs
#define PID_ACK_HANDSHAKE   0b0010
#define PID_NAK_HANDSHAKE   0b1010
#define PID_STALL_HANDSHAKE 0b1110
#define PID_NYET_HANDSHAKE  0b0110

// Special PIDs
#define PID_PREamble 0b1100
#define PID_ERR      0b1100
#define PID_Split    0b1000
#define PID_Ping     0b0100
/****************************************/

#include "usb_hal.h"
#include <stdint.h>
#include <stdbool.h>

/** Device Settings. */
struct{
    unsigned Self_Powered  :1;
    unsigned Remote_Wakeup :1;
    unsigned               :6;
}DEVS;

/** EP structures for OUT and IN status. */
typedef struct{
    unsigned Data_Toggle_Val :1;
    unsigned Halt            :1;
    unsigned Last_PPB        :1;
    unsigned                 :5;
}EPS_Type;

union{
    uint8_t BYTE;
    struct{
        unsigned      :1;
        unsigned PPBI :1;
        unsigned DIR  :1;
        unsigned ENDP :4;
        unsigned      :1;
    };
}LastUSTAT;

/** EP status. */
#define EPn_SIZE (NUM_ENDPOINTS*2)
union{
    uint8_t Clear[EPn_SIZE];
    EPS_Type Array[NUM_ENDPOINTS][2];
}EPn_Stat;

/** EP0 status HAL */
#define EP0_OUT_LAST_PPB EPn_Stat.Array[EP0][OUT].Last_PPB
#define EP0_IN_LAST_PPB  EPn_Stat.Array[EP0][IN].Last_PPB
#define EP0_OUT_DATA_TOGGLE_VAL EPn_Stat.Array[EP0][OUT].Data_Toggle_Val
#define EP0_IN_DATA_TOGGLE_VAL  EPn_Stat.Array[EP0][IN].Data_Toggle_Val

#ifdef _PIC14E
#define EP1CONDIS EPCONDIS
#define EP0CONDIS EPCONDIS
#endif

#if (PINGPONG_MODE != PINGPONG_ALL_EP)
#define USB_RequestError() USB_Stall_EP(&BDT.Array[BD0_IN])
#else
#define USB_RequestError() USB_Stall_EP(&BDT.Array[BD0_IN_EVEN]);USB_Stall_EP(&BDT.Array[BD0_IN_ODD])
#endif

void USB_Tasks(void);
void USB_Init(void);
void USB_Close(void);

void USB_ArmEndpoint(BD* BD_ptr, EPS_Type* EPS_ptr, uint16_t buffer_addr, uint16_t cnt);
#if PINGPONG_MODE == PINGPONG_ALL_EP
void USB_Arm_EP0_IN(uint8_t BDT_Index, uint16_t cnt);
#else
void USB_Arm_EP0_IN(uint16_t cnt);
#endif
void USB_ArmStatus(BD* BD_ptr);
void USB_ArmINStatus(void);

void USB_Stall_EP0(void);
void USB_Stall_EP(BD* BD_ptr);

void USB_OutControlTransfer(void);
void USB_OutControlStatus(void);
void USB_SetupInControlTransfer(uint16_t addr, uint16_t bytes_available, uint16_t bytes_requested);
void USB_InControlTransfer(void);
void USB_InControlStatus(void);

void USB_ROMCopy(const uint8_t *ROMaddress, uint8_t *EPaddress, uint8_t bytes);
void USB_RAMCopy(uint8_t *RAM1address, uint8_t *RAM2address, uint8_t bytes);
void USB_RAMSet(uint8_t val, uint8_t *RAMaddress, uint16_t bytes);

void USB_Reset(void);
void USB_Error(void);
void USB_Idle(void);
void USB_Activity(void);
void USB_SOF(void);
bool USB_OutControlFinished(void);

volatile uint8_t USB_STATE = Detached;
uint8_t CurrentConfiguration;
uint8_t ControlStage;

#define ROM 0
#define RAM 1
uint16_t bytes_available;
uint16_t bytes_2_send;
uint16_t bytes_2_recv;
bool send_short;
uint8_t SendingFrom;
const uint8_t *ROMptr;
uint8_t *RAMptr;

#endif /* USB_H */