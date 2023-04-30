/**
 * @file usb_cdc.h
 * @brief <i>Communications Device Class</i> core header file.
 * @author John Izzard
 * @date 30/04/2023
 * 
 * USB uC - CDC Library.
 * Copyright (C) 2017-2021  John Izzard
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

#ifndef CDC_H
#define CDC_H

#include <stdint.h>
#include <stdbool.h>
#include "usb_config.h"
#include "usb_cdc_config.h"
#include "usb_hal.h"

/* ************************************************************************** */
/* ******************** SET LINE CODING SETTING DEFINES ********************* */
/* ************************************************************************** */

#define STOP_BIT_1   0
#define STOP_BIT_1_5 1
#define STOP_BIT_2   2
#define PARITY_NONE  0
#define PARITY_ODD   1
#define PARITY_EVEN  2
#define PARITY_MARK  3
#define PARITY_SPACE 4

/* ************************************************************************** */

/* ************************************************************************** */
/* ************************** WARNING FOR PIC16 ***************************** */
/* ************************************************************************** */

#ifdef _PIC14E
#warning "CDC EP Buffer addresses have been manually set for PIC16 devices."
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* ************************** WARNING FOR PIC14 ***************************** */
/* ************************************************************************** */

#if PINGPONG_MODE == PINGPONG_DIS
#define CDC_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*2))
#elif PINGPONG_MODE == PINGPONG_0_OUT
#define CDC_EP_BUFFERS_STARTING_ADDR  (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*3))
#else
#error "No point to pingpong buffering CDC's DATA EP. As transfer length is not known beforhand, there's no advantage. PINGPONG_0_OUT is recommended."
#endif


/* ************************************************************************** */
/* ************************** CDC EP ADDRESSES ****************************** */
/* ************************************************************************** */

#ifdef _PIC14E
#define CDC_COM_EP_IN_BUFFER_BASE_ADDR  0x2050
#define CDC_DAT_EP_OUT_BUFFER_BASE_ADDR 0x20A0
#define CDC_DAT_EP_IN_BUFFER_BASE_ADDR  0x20F0
#else
#define CDC_COM_EP_IN_BUFFER_BASE_ADDR   CDC_EP_BUFFERS_STARTING_ADDR
#define CDC_DAT_EP_OUT_BUFFER_BASE_ADDR (CDC_EP_BUFFERS_STARTING_ADDR + CDC_COM_EP_SIZE)
#define CDC_DAT_EP_IN_BUFFER_BASE_ADDR  (CDC_EP_BUFFERS_STARTING_ADDR + CDC_COM_EP_SIZE + CDC_DAT_EP_SIZE)
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* *********************** EP STAT TOGGLE VAL HAL *************************** */
/* ************************************************************************** */

#define CDC_COM_EP_IN_DATA_TOGGLE_VAL  g_usb_ep_stat[CDC_COM_EP][IN].Data_Toggle_Val
#define CDC_DAT_EP_OUT_DATA_TOGGLE_VAL g_usb_ep_stat[CDC_DAT_EP][OUT].Data_Toggle_Val
#define CDC_DAT_EP_IN_DATA_TOGGLE_VAL  g_usb_ep_stat[CDC_DAT_EP][IN].Data_Toggle_Val

/* ************************************************************************** */


/* ************************************************************************** */
/* *************************** CDC CLASS CODES ****************************** */
/* ************************************************************************** */

// Communication Device Class Code
#define CDC_CLASS 0x02

// Communication Interface Class Code
#define CIC_CODE 0x02

// Communication Interface Class SubClass Codes
#define CIC_DLCM     0x01
#define CIC_ACM      0x02
#define CIC_TCM      0x03
#define CIC_MCCM     0x04
#define CIC_CAPI_CM  0x05
#define CIC_ENCM     0x06
#define CIC_ATM_NCM  0x07

// Communication Interface Class Control Protocol Codes
#define CIC_V25TER 0x01
#define CIC_VENDOR 0xFF

// Data Interface Class Code
#define DIC_CODE   0x0A

// Data Interface Class Protocol Codes
#define DIC_NONE        0x00
#define DIC_I430        0x30
#define DIC_HDLC        0x31
#define DIC_TRANSPARENT 0x32
#define DIC_Q921M       0x50
#define DIC_Q921        0x51
#define DIC_Q921TM      0x52
#define DIC_V42BIS      0x90
#define DIC_Q931        0x91
#define DIC_V120        0x92
#define DIC_CAPI20      0x93
#define DIC_HOST_DRIVER 0xFD
#define DIC_PUFD        0xFE
#define DIC_VENDOR      0xFF

/* ************************************************************************** */


/* ************************************************************************** */
/* *********************** CDC DESCRIPTOR DEFINES *************************** */
/* ************************************************************************** */

// Type Values for the bDescriptorType Field
#define CS_INTERFACE    0x24
#define CS_ENDPOINT     0x25

// bDescriptor SubType in Functional Descriptors
#define DESC_SUB_HEADER  0x00
#define DESC_SUB_CM      0x01
#define DESC_SUB_ACM     0x02
#define DESC_SUB_DLM     0x03
#define DESC_SUB_TR      0x04
#define DESC_SUB_TCLSRC  0x05
#define DESC_SUB_UNION   0x06
#define DESC_SUB_CNTRY   0x07
#define DESC_SUB_TOM     0x08
#define DESC_SUB_USB_T   0x09
#define DESC_SUB_NCT     0x0A
#define DESC_SUB_PU      0x0B
#define DESC_SUB_EU      0x0C
#define DESC_SUB_MCM     0x0D
#define DESC_SUB_CAPI_CM 0x0E
#define DESC_SUB_EN      0x0F
#define DESC_SUB_ATM_N   0x10

/* ************************************************************************** */


/* ************************************************************************** */
/* **************************** CDC REQUESTS ******************************** */
/* ************************************************************************** */

// CDC Requests
#define SEND_ENCAPSULATED_COMMAND 0x00
#define GET_ENCAPSULATED_RESPONSE 0x01
#define SET_COMM_FEATURE          0x02
#define GET_COMM_FEATURE          0x03
#define CLEAR_COMM_FEATURE        0x04
#define SET_AUX_LINE_STATE        0x10
#define SET_HOOK_STATE            0x11
#define PULSE_SETUP               0x12
#define SEND_PULSE                0x13
#define SET_PULSE_TIME            0x14
#define RING_AUX_JACK             0x15
#define SET_LINE_CODING           0x20
#define GET_LINE_CODING           0x21
#define SET_CONTROL_LINE_STATE    0x22
#define SEND_BREAK                0x23
#define SET_RINGER_PARMS          0x30
#define GET_RINGER_PARMS          0x31
#define SET_OPERATIONS_PARMS      0x32
#define GET_OPERATIONS_PARMS      0x33
#define SET_LINE_PARMS            0x34
#define GET_LINE_PARMS            0x35
#define DIAL_DIGITS               0x36
#define SET_UNIT_PARAMETER        0x37
#define GET_UNIT_PARAMETER        0x38
#define CLEAR_UNIT_PARAMETER      0x39
#define GET_PROFILE               0x3A
#define SET_ETHERNET_MULTICAST_FILTERS              0x40
#define SET_ETHERNET_POWER_MANAGMENT_PATTERN_FILTER 0x41
#define GET_ETHERNET_POWER_MANAGMENT_PATTERN_FILTER 0x42
#define SET_ETHERNET_PACKET_FILTER 0x43
#define GET_ETHERNET_STATISTIC     0x44
#define SET_ATM_DATA_FORMAT        0x50
#define GET_ATM_DEVICE_STATISTIC   0x51
#define SET_ATM_DEFAULT_VC         0x52
#define GET_ATM_VC_STATISTICS      0x53

/* ************************************************************************** */


/* ************************************************************************** */
/* **************************** NOTIFICATION CODES ************************** */
/* ************************************************************************** */

// Class-Specific Notification Codes
#define NETWORK_CONNECTION      0x00
#define RESPONSE_AVAILABLE      0x01
#define AUX_JACK_HOOK_STATE     0x08
#define RING_DETECT             0x09
#define SERIAL_STATE            0x20
#define CALL_STATE_CHANGE       0x28
#define LINE_STATE_CHANGE       0x29
#define CONNECTION_SPEED_CHANGE 0x2A

/* ************************************************************************** */


/* ************************************************************************** */
/* ********************************* TYPES ********************************** */
/* ************************************************************************** */

// Header Functional Descriptor
typedef struct
{
    uint8_t  bFunctionLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint16_t bcdCDC;
}cdc_header_functional_descriptor_t;

// Call Management Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
    uint8_t bDataInterface;
}cdc_cm_functional_descriptor_t;

// Abstract Control Management Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
}cdc_acm_functional_descriptor_t;

// Direct Line Management Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
}cdc_dlm_functional_descriptor_t;

// Telephone Ringer Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bRingerVolSteps;
    uint8_t bNumRingerPattern;
}cdc_tr_functional_descriptor_t;

// Telephone Operational Modes Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
}cdc_tom_functional_descriptor_t;

// Telephone Call and Line State Reporting Capabilities Descriptor
typedef struct
{
    uint8_t  bFunctionLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint32_t bmCapabilities;
}cdc_tclsr_capabilities_desctriptor_t;

// Union Functional Descriptor
#define NUM_SLAVE_INTERFACES 1
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bMasterInterface;
    uint8_t bSlaveInterface[NUM_SLAVE_INTERFACES];
}cdc_union_functional_descriptor_t;

// Country Selection Functional Descriptor
#define NUM_COUNTRY_CODES 1
typedef struct
{
    uint8_t  bFunctionLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  iCountryCodeRelDate;
    uint16_t wCountryCode[NUM_COUNTRY_CODES];
}cdc_cs_functional_descriptor_t;

// USB Terminal Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t bInInterfaceNo;
    uint8_t bOutInterfaceNo;
    uint8_t bmOptions;
    uint8_t bChildId[1];
}cdc_usb_terminal_descriptor_t;

// Network Channel Terminal Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t iName;
    uint8_t bChannelIndex;
    uint8_t bPhysicalIndex;
}cdc_nct_functional_descriptor_t;

// Protocol Unit Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t bProtocol;
    uint8_t bChildId[1];
}cdc_pu_functional_descriptor_t;

// Extension Unit Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t bExtentionCode;
    uint8_t iName;
    uint8_t bChildId[1];
}cdc_eu_functional_descriptor_t;

// Multi-Channel Management Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
}cdc_mcm_functional_descriptor_t;

// CAPI Control Management Functional Descriptor
typedef struct
{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
}cdc_capi_cm_functional_descriptor_t;

// Ethernet Networking Functional Descriptor
typedef struct
{
    uint8_t  bFunctionLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  iMACAddress;
    uint32_t bmEthernetStatistics;
    uint16_t wMaxSegmentSize;
    uint16_t wNumberMCFilters;
    uint8_t  bNumberPowerFilters;
}cdc_en_functional_descriptor_t;

// ATM Networking Functional Descriptor
typedef struct
{
    uint8_t  bFunctionLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  iEndSystemIdentifier;
    uint8_t  bmDataCapabilities;
    uint8_t  bmATMDeviceStatistics;
    uint16_t wType2MaxSegmentSize;
    uint16_t wType3MaxSegmentSize;
    uint16_t wMaxVC;
}cdc_atm_networking_functional_descriptor_t;

typedef struct
{
    uint32_t dwDTERate;
    uint8_t  bCharFormat;
    uint8_t  bParityType;
    uint8_t  bDataBits;
}cdc_get_line_coding_return_t;

typedef struct
{
    uint32_t dwDTERate;
    uint8_t  bCharFormat;
    uint8_t  bParityType;
    uint8_t  bDataBits;
}cdc_set_line_coding_t;

typedef struct
{
    unsigned :8;
    unsigned :8;
    uint16_t Zero;
    uint16_t Interface;
    uint16_t Size_of_Structure;
}cdc_set_get_line_coding_t;

typedef struct
{
    unsigned :8;
    unsigned :8;
    unsigned DTR_bit :1;
    unsigned RTS_bit :1;
    unsigned :6;
    unsigned :8;
    uint8_t  Zero;
}cdc_set_control_line_state_t;

typedef union
{
    uint8_t array[10];
    struct
    {
        union
        {
            uint8_t array[8];
            struct{
                uint8_t  bmRequestType;
                uint8_t  bNotification;
                uint16_t wValue;
                uint16_t wIndex;
                uint16_t wLength;
            };
        }header;
        unsigned bRxCarrier  :1;
        unsigned bTxCarrier  :1;
        unsigned bBreak      :1;
        unsigned bRingSignal :1;
        unsigned bFraming    :1;
        unsigned bParity     :1;
        unsigned bOverRun    :1;
        unsigned             :1;
        unsigned             :8;
    };
}cdc_serial_state_t;

/* ************************************************************************** */


/* ************************************************************************** */
/* ******************** VARIABLES FROM: usb_cdc_acm.c *********************** */
/* ************************************************************************** */

// Glabal Variables To Share From usb_cdc_acm.c
extern uint8_t g_cdc_com_ep_in[CDC_COM_EP_SIZE]  __at(CDC_COM_EP_IN_BUFFER_BASE_ADDR);
extern uint8_t g_cdc_dat_ep_out[CDC_DAT_EP_SIZE] __at(CDC_DAT_EP_OUT_BUFFER_BASE_ADDR);
extern uint8_t g_cdc_dat_ep_in[CDC_DAT_EP_SIZE]  __at(CDC_DAT_EP_IN_BUFFER_BASE_ADDR);

extern cdc_set_get_line_coding_t    g_cdc_set_get_line_coding       __at(SETUP_DATA_ADDR);
extern cdc_set_control_line_state_t g_cdc_set_control_line_state    __at(SETUP_DATA_ADDR);
extern cdc_get_line_coding_return_t g_cdc_get_line_coding_return;
extern cdc_set_line_coding_t        g_cdc_set_line_coding;
#if defined(USE_DTR)||defined(USE_RTS)
extern cdc_serial_state_t           g_cdc_serial_state              __at(CDC_COM_EP_IN_BUFFER_BASE_ADDR);
#endif

extern volatile bool    g_cdc_set_line_coding_wait;
extern volatile uint8_t g_cdc_num_data_out;

#if defined(USE_DTR)||defined(USE_DCD)
extern bool g_cdc_sent_last_notification;
extern bool g_cdc_send_notification;
#endif
#ifdef USE_RTS
extern bool g_cdc_has_set_rts;
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* *************************** NOTIFICATION TASKS *************************** */
/* ************************************************************************** */

#if defined(USE_DTR)||defined(USE_DCD)
void cdc_notification_tasks(void);
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* *************************** CDC FUNCTIONS ******************************** */
/* ************************************************************************** */

bool cdc_class_request(void);
void cdc_init(void);
void cdc_clear_ep_toggle(void);
bool cdc_out_control_tasks(void);
void cdc_set_line_coding(void);
void cdc_set_control_line_state(void);
void cdc_tasks(void);
void cdc_data_out(void);
void cdc_data_in(void);
void cdc_notification(void);

/**
 * @fn void cdc_arm_com_ep_in(void)
 * 
 * @brief Arms CDC COM Endpoint for a transaction.
 * 
 * The function is used to arm CDC COM EP for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * cdc_arm_com_ep_in();
 * @endcode
 * </li></ul>
 */
void cdc_arm_com_ep_in(void);

/**
 * @fn cdc_arm_data_ep_out(void)
 * 
 * @brief Arms CDC DAT EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm CDC DAT EP OUT for a transaction.
 * 
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * cdc_arm_data_ep_out();
 * @endcode
 * </li></ul>
 */
void cdc_arm_data_ep_out(void);

/**
 * @fn void cdc_arm_data_ep_in(uint8_t cnt)
 * 
 * @brief Arms CDC DAT EP IN Endpoint for a transaction.
 * 
 * The function is used to arm CDC DAT EP IN for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * cdc_arm_data_ep_in(CDC_DAT_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void cdc_arm_data_ep_in(uint8_t cnt);

/* ************************************************************************** */

#endif