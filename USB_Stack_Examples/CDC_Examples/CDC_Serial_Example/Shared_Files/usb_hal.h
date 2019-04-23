/**
 * @file usb_hal.h
 * @brief <i>Hardware Abstraction Layer</i> for Endpoints.
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
#ifndef USB_HAL_H
#define USB_HAL_H

#include <xc.h>
#include <stdint.h>
#include "usb_config.h"

/******************* UEPn Register Bits *******************/
#define _EPHSHK    0x10
#define _EPCONDIS  0x08
#define _EPOUTEN   0x04
#define _EPINEN    0x02
#define _EPSTALL   0x01
/**********************************************************/

/******************* BD STAT Register Bits ****************/
#define _UOWN   0x80
#define _DTS    0x40
#define _DTSEN  0x08
#define _BSTALL 0x04
/**********************************************************/

/******************* UCFG Register Bits *******************/
#define _UTEYE 0x80
#define _UPUEN 0x10
#define _FSEN  0x04
#define _PPB1  0x02
#define _PPB0  0x01

#define PPB PINGPONG_MODE
/**********************************************************/

/******************* UIE Register Bits ********************/
#define _SOFIE   0x40
#define _STALLIE 0x20
#define _IDLEIE  0x10
#define _TRNIE   0x08
#define _ACTVIE  0x04
#define _UERIE   0x02
#define _URSTIE  0x01
/**********************************************************/

/******************* UEIE Register Bits ********************/
#define _BTSEE   0x80
#define _BTOEE   0x10
#define _DFN8EE  0x08
#define _CRC16EE 0x04
#define _CRC5EE  0x02
#define _PIDEE   0x01
/**********************************************************/

/***************** Defines for USTAT ****************************/
#define IN  1
#define OUT 0

#define EP0  0
#define EP1  1
#define EP2  2
#define EP3  3
#define EP4  4
#define EP5  5
#define EP6  6
#define EP7  7
#define EP8  8
#define EP9  9
#define EP10 10
#define EP11 11
#define EP12 12
#define EP13 13
#define EP14 14
#define EP15 15
/**********************************************************************/

/********* Defines for Explicit Access to Buffer Descriptors **********/
#define EVEN 0
#define ODD  1
#if (PINGPONG_MODE == PINGPONG_DIS)
#define BD0_OUT   0u
#define BD0_IN    1u
#define BD1_OUT   2u
#define BD1_IN    3u
#if NUM_ENDPOINTS > 2
#define BD2_OUT   4u
#define BD2_IN    5u
#endif
#elif (PINGPONG_MODE == PINGPONG_0_OUT)
#define BD0_OUT_EVEN  0u
#define BD0_OUT_ODD   1u
#define BD0_IN        2u
#define BD1_OUT       3u
#define BD1_IN        4u
#if NUM_ENDPOINTS > 2
#define BD2_OUT       5u
#define BD2_IN        6u
#endif
#elif (PINGPONG_MODE == PINGPONG_1_15)
#define BD0_OUT      0u
#define BD0_IN       1u
#define BD1_OUT_EVEN 2u
#define BD1_OUT_ODD  3u
#define BD1_IN_EVEN  4u
#define BD1_IN_ODD   5u
#if NUM_ENDPOINTS > 2
#define BD2_OUT_EVEN 6u
#define BD2_OUT_ODD  7u
#define BD2_IN_EVEN  8u
#define BD2_IN_ODD   9u
#endif
#elif (PINGPONG_MODE == PINGPONG_ALL_EP)
#define BD0_OUT_EVEN  0u
#define BD0_OUT_ODD   1u
#define BD0_IN_EVEN   2u
#define BD0_IN_ODD    3u
#define BD1_OUT_EVEN  4u
#define BD1_OUT_ODD   5u
#define BD1_IN_EVEN   6u
#define BD1_IN_ODD    7u
#if NUM_ENDPOINTS > 2
#define BD2_OUT_EVEN  8u
#define BD2_OUT_ODD   9u
#define BD2_IN_EVEN   10u
#define BD2_IN_ODD    11u
#endif
#endif
/**********************************************************************/

/************* Buffer Descriptor Size Calculation  ********************/
#if (PINGPONG_MODE == PINGPONG_DIS)
#define NUM_BD (NUM_ENDPOINTS*2)
#elif (PINGPONG_MODE == PINGPONG_0_OUT)
#define NUM_BD ((NUM_ENDPOINTS*2)+1)
#elif (PINGPONG_MODE == PINGPONG_1_15)
#define NUM_BD ((NUM_ENDPOINTS*4)-2)
#else
#define NUM_BD (NUM_ENDPOINTS*4)
#endif
#define BDT_SIZE (NUM_BD*4)
/**********************************************************************/

/******************* Processor Specific Defines  **********************/
#if defined(_PIC14E)
#define BDT_BASE_ADDR            0x2000
#define SETUP_DATA_ADDR          0x70
#elif defined(_18F13K50) || defined(_18F14K50)
#define BDT_BASE_ADDR   0x200
#define SETUP_DATA_ADDR 0x60
#define EP_BUFFERS_STARTING_ADDR (BDT_BASE_ADDR + BDT_SIZE)
#elif defined(_18F24K50)||defined(_18F25K50)||defined(_18F45K50)
#define BDT_BASE_ADDR   0x400
#define SETUP_DATA_ADDR 0x60
#define EP_BUFFERS_STARTING_ADDR (BDT_BASE_ADDR + BDT_SIZE)
#elif defined(__J_PART)
#define BDT_BASE_ADDR 0xD00
#define SETUP_DATA_ADDR 0x60
#define EP_BUFFERS_STARTING_ADDR (BDT_BASE_ADDR + BDT_SIZE)
#endif

#if defined(_18F24K50)||defined(_18F25K50)||defined(_18F45K50)
#define USB_Interrupt_Enable PIE3bits.USBIE
#define USB_Interrupt_Flag   PIR3bits.USBIF
#else
#define USB_Interrupt_Enable PIE2bits.USBIE
#define USB_Interrupt_Flag   PIR2bits.USBIF
#endif

#define ResetConditionFlag UIRbits.URSTIF
#define ErrorConditionFlag UIRbits.UERRIF
#define ActivityDetectFlag UIRbits.ACTVIF
#define TransactionCompleteFlag UIRbits.TRNIF
#define IdleDetectFlag UIRbits.IDLEIF
#define StallConditionFlag UIRbits.STALLIF
#define SOF_Flag UIRbits.SOFIF

#define ActivityDetectEnable UIEbits.ACTVIE

#define USB_ControlRegister UCON
#define USB_ConfigurationRegister UCFG
#define USB_EP0_ControlRegister UEP0
#define USB_EP1_ControlRegister UEP1
#define USB_EP2_ControlRegister UEP2
#define USB_EP3_ControlRegister UEP3
#define USB_EP4_ControlRegister UEP4
#define USB_EP5_ControlRegister UEP5
#define USB_EP6_ControlRegister UEP6
#define USB_EP7_ControlRegister UEP7
#define USB_InterruptStatRegister UIR
#define USB_InterruptEnableRegister UIE
#define USB_ErrorInterruptStatRegister UEIR
#define USB_ErrorInterruptEnableRegister UEIE
#define PacketTransferDisable UCONbits.PKTDIS
#define USB_ModuleEnable UCONbits.USBEN
#define SingleEndedZero UCONbits.SE0
#define PPB_Reset UCONbits.PPBRST
#define USB_Suspend UCONbits.SUSPND

#if defined(_18F13K50) || defined(_18F14K50)
struct { // PIC18F14K50.h is outdated
    unsigned                        :3;
    unsigned ENDP                   :4;
}_USTATbits @ 0xF63;
#else
#define _USTATbits USTATbits
#endif

//#define TRANSACTION_EP  _USTATbits.ENDP
//#define TRANSACTION_DIR USTATbits.DIR
//#define PINGPONG_PARITY USTATbits.PPBI
#define TRANSACTION_EP  LastUSTAT.ENDP
#define TRANSACTION_DIR LastUSTAT.DIR
#define PINGPONG_PARITY LastUSTAT.PPBI
/**********************************************************************/

/** 
 * @var BDT
 * @brief Buffer Descriptor Table Structure. 
 */
typedef struct{
    union{
        struct{   
            union{
                struct{
                    unsigned BC8:1;
                    unsigned BC9:1;
                    unsigned BSTALL:1;
                    unsigned DTSEN:1;
                    unsigned :1;
                    unsigned :1;
                    unsigned DTS:1;
                    unsigned UOWN:1;
                };
                struct{
                    unsigned :2;
                    unsigned PID0:1;
                    unsigned PID1:1;
                    unsigned PID2:1;
                    unsigned PID3:1;
                    unsigned :2;
                };
                struct{
                    unsigned :2;
                    unsigned PID:4;
                    unsigned :2;
                };
            }STATbits;
            uint8_t  CNT;
            uint8_t  ADRL;
            uint8_t  ADRH;
        };
        struct{
            uint8_t  STAT;
            unsigned :8;
            uint16_t ADR;
        };
    };
}BD;

union{
    BD Array[NUM_BD];
    uint8_t Clear[BDT_SIZE];
}BDT @BDT_BASE_ADDR;

/**************** Setup EP0 Buffer Base Addresses ************************/
#ifdef _PIC14E
#warning "Control EP Buffer addresses need to be manually set for PIC16 devices."
#if (PINGPONG_MODE == PINGPONG_DIS) || (PINGPONG_MODE == PINGPONG_1_15)
#if EP0_SIZE == 8
#define EP0_OUT_BUFFER_BASE_ADDR 0x21E0
#define EP0_IN_BUFFER_BASE_ADDR  0x21E8
#elif EP0_SIZE == 16
#define EP0_OUT_BUFFER_BASE_ADDR 0x21D0
#define EP0_IN_BUFFER_BASE_ADDR  0x21E0
#elif EP0_SIZE == 32
#define EP0_OUT_BUFFER_BASE_ADDR 0x21A0
#define EP0_IN_BUFFER_BASE_ADDR  0x21C0
#else //EP0_SIZE == 64
#define EP0_OUT_BUFFER_BASE_ADDR 0x2150
#define EP0_IN_BUFFER_BASE_ADDR  0x2190
#endif
#elif (PINGPONG_MODE == PINGPONG_0_OUT)
#if EP0_SIZE == 8
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x21D8
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x21E0
#define EP0_IN_BUFFER_BASE_ADDR       0x21E8
#elif EP0_SIZE == 16
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x21C0
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x21D0
#define EP0_IN_BUFFER_BASE_ADDR       0x21E0
#elif EP0_SIZE == 32
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x2170
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x2190
#define EP0_IN_BUFFER_BASE_ADDR       0x21B0
#else //EP0_SIZE == 64
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x20F0
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x2140
#define EP0_IN_BUFFER_BASE_ADDR       0x2190
#endif
#else
#if EP0_SIZE == 8
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x21D0
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x21D8
#define EP0_IN_EVEN_BUFFER_BASE_ADDR  0x21E0
#define EP0_IN_ODD_BUFFER_BASE_ADDR   0x21E8
#elif EP0_SIZE == 16
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x21B0
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x21C0
#define EP0_IN_EVEN_BUFFER_BASE_ADDR  0x21D0
#define EP0_IN_ODD_BUFFER_BASE_ADDR   0x21E0
#elif EP0_SIZE == 32
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x2150
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x2170
#define EP0_IN_EVEN_BUFFER_BASE_ADDR  0x2190
#define EP0_IN_ODD_BUFFER_BASE_ADDR   0x21B0
#else //EP0_SIZE == 64
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR 0x20A0
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  0x20F0
#define EP0_IN_EVEN_BUFFER_BASE_ADDR  0x2140
#define EP0_IN_ODD_BUFFER_BASE_ADDR   0x2190
#endif
#endif
#else // PIC18 devices
#if (PINGPONG_MODE == PINGPONG_DIS)
#define EP0_BUFFER_BASE_ADDR EP_BUFFERS_STARTING_ADDR
#define EP0_OUT_BUFFER_BASE_ADDR EP0_BUFFER_BASE_ADDR
#define EP0_IN_BUFFER_BASE_ADDR  (EP0_BUFFER_BASE_ADDR+EP0_SIZE)

#elif (PINGPONG_MODE == PINGPONG_0_OUT)
#define EP0_BUFFER_BASE_ADDR EP_BUFFERS_STARTING_ADDR
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR EP0_BUFFER_BASE_ADDR
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  (EP0_BUFFER_BASE_ADDR+EP0_SIZE)
#define EP0_IN_BUFFER_BASE_ADDR       (EP0_BUFFER_BASE_ADDR+(EP0_SIZE*2))

#elif (PINGPONG_MODE == PINGPONG_1_15)
#define EP0_BUFFER_BASE_ADDR EP_BUFFERS_STARTING_ADDR
#define EP0_OUT_BUFFER_BASE_ADDR      EP0_BUFFER_BASE_ADDR
#define EP0_IN_BUFFER_BASE_ADDR       (EP0_BUFFER_BASE_ADDR+EP0_SIZE)

#else
#define EP0_BUFFER_BASE_ADDR EP_BUFFERS_STARTING_ADDR
#define EP0_OUT_EVEN_BUFFER_BASE_ADDR EP0_BUFFER_BASE_ADDR
#define EP0_OUT_ODD_BUFFER_BASE_ADDR  (EP0_BUFFER_BASE_ADDR+EP0_SIZE)
#define EP0_IN_EVEN_BUFFER_BASE_ADDR  (EP0_BUFFER_BASE_ADDR+(EP0_SIZE*2))
#define EP0_IN_ODD_BUFFER_BASE_ADDR   (EP0_BUFFER_BASE_ADDR+(EP0_SIZE*3))
#endif
#endif
/**************************************************************************/

/************************** Setup EP0 Buffers *****************************/
#if (PINGPONG_MODE == PINGPONG_DIS) || (PINGPONG_MODE == PINGPONG_1_15)

uint8_t EP0_OUT[EP0_SIZE] @EP0_OUT_BUFFER_BASE_ADDR;
uint8_t EP0_IN[EP0_SIZE]  @EP0_IN_BUFFER_BASE_ADDR;

#elif (PINGPONG_MODE == PINGPONG_0_OUT)

uint8_t EP0_OUT_EVEN[EP0_SIZE] @EP0_OUT_EVEN_BUFFER_BASE_ADDR;
uint8_t EP0_OUT_ODD[EP0_SIZE]  @EP0_OUT_ODD_BUFFER_BASE_ADDR;
uint8_t EP0_IN[EP0_SIZE]       @EP0_IN_BUFFER_BASE_ADDR;

#else

uint8_t EP0_OUT_EVEN[EP0_SIZE]    @EP0_OUT_EVEN_BUFFER_BASE_ADDR;
uint8_t EP0_OUT_ODD[EP0_SIZE]     @EP0_OUT_ODD_BUFFER_BASE_ADDR;
uint8_t EP0_IN_EVEN[EP0_SIZE]     @EP0_IN_EVEN_BUFFER_BASE_ADDR;
uint8_t EP0_IN_ODD[EP0_SIZE]      @EP0_IN_ODD_BUFFER_BASE_ADDR;

#endif
/*************************************************************************/

#endif /* USB_HAL_H */