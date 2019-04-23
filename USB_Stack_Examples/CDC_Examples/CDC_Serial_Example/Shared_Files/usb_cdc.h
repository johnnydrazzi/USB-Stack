/**
 * @file CDC.h
 * @brief <i>Communications Device Class</i> settings, structures, global variable and function declarations.
 * @author John Izzard
 * @date 13/03/2019
 * 
 * USB uC - CDC Library.
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
#ifndef CDC_H
#define CDC_H

#include <stdint.h>
#include <stdbool.h>

// Settings
#define STARTING_BAUD 9600

#define STOP_BIT_1   0
#define STOP_BIT_1_5 1
#define STOP_BIT_2   2
#define STARTING_STOP_BITS STOP_BIT_1

#define PARITY_NONE  0
#define PARITY_ODD   1
#define PARITY_EVEN  2
#define PARITY_MARK  3
#define PARITY_SPACE 4
#define STARTING_PARITY PARITY_NONE

#define STARTING_DATA_BITS 8 // 5, 6, 7, 8, or 16.

//#define USE_SET_COMM_FEATURE   // Not yet implemented.
//#define USE_GET_COMM_FEATURE   // Not yet implemented.
//#define USE_CLEAR_COMM_FEATURE // Not yet implemented.
#define USE_SET_LINE_CODING
#define USE_GET_LINE_CODING
#define USE_SET_CONTROL_LINE_STATE
//#define USE_SEND_BREAK         // Not yet implemented.

// CDC Endpoint HAL
#define CDC_COM_EP EP1
#define CDC_DAT_EP EP2
#define CDC_COM_EP_SIZE EP1_SIZE
#define CDC_DAT_EP_SIZE EP2_SIZE

#ifdef _PIC14E
#warning "CDC EP Buffer addresses need to be manually set for PIC16 devices."
#endif

#if PINGPONG_MODE == PINGPONG_DIS
#define CDC_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*2))
#elif PINGPONG_MODE == PINGPONG_0_OUT
#define CDC_EP_BUFFERS_STARTING_ADDR  (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE*3))
#else
#error "No point to pingpong buffering CDC's DATA EP. As transfer length is not known beforhand, there's no advantage. PINGPONG_0_OUT is recommended."
#endif

#ifdef _PIC14E
#define CDC_COM_EP_IN_BUFFER_BASE_ADDR  0x2050
#define CDC_DAT_EP_OUT_BUFFER_BASE_ADDR 0x20A0
#define CDC_DAT_EP_IN_BUFFER_BASE_ADDR  0x20F0
#else
#define CDC_COM_EP_IN_BUFFER_BASE_ADDR  CDC_EP_BUFFERS_STARTING_ADDR
#define CDC_DAT_EP_OUT_BUFFER_BASE_ADDR (CDC_EP_BUFFERS_STARTING_ADDR + CDC_COM_EP_SIZE)
#define CDC_DAT_EP_IN_BUFFER_BASE_ADDR  (CDC_EP_BUFFERS_STARTING_ADDR + CDC_COM_EP_SIZE + CDC_DAT_EP_SIZE)
#endif
uint8_t CDC_COM_EP_IN[CDC_COM_EP_SIZE]  @CDC_COM_EP_IN_BUFFER_BASE_ADDR;
uint8_t CDC_DAT_EP_OUT[CDC_DAT_EP_SIZE] @CDC_DAT_EP_OUT_BUFFER_BASE_ADDR;
uint8_t CDC_DAT_EP_IN[CDC_DAT_EP_SIZE]  @CDC_DAT_EP_IN_BUFFER_BASE_ADDR;

#define CDC_COM_BD_IN BD1_IN
#define CDC_DAT_BD_OUT BD2_OUT
#define CDC_DAT_BD_IN BD2_IN

#define CDC_COM_UEPbits UEP1bits
#define CDC_DAT_UEPbits UEP2bits

#define CDC_COM_EP_IN_DATA_TOGGLE_VAL  EPn_Stat.Array[CDC_COM_EP][IN].Data_Toggle_Val
#define CDC_DAT_EP_OUT_DATA_TOGGLE_VAL EPn_Stat.Array[CDC_DAT_EP][OUT].Data_Toggle_Val
#define CDC_DAT_EP_IN_DATA_TOGGLE_VAL  EPn_Stat.Array[CDC_DAT_EP][IN].Data_Toggle_Val

//#define USE_DCD
#define DCD_ACTIVE 0
#define DCD PORTBbits.RB0

//#define USE_DTR
#define DTR_ACTIVE 0
#define DSR_ACTIVE 0
#define DTR LATBbits.LATB1
#define DSR PORTBbits.RB2
#define DTR_TRIS TRISBbits.TRISB1

//#define USE_RTS
#define RTS_ACTIVE 0
#define CTS_ACTIVE 0
#define RTS LATBbits.LATB3
#define CTS PORTBbits.RB4
#define RTS_TRIS TRISBbits.TRISB3

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
#define DIC_CODE 0x0A

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

// Type Values for the bDescriptorType Field
#define CS_INTERFACE 0x24
#define CS_ENDPOINT  0x25

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

// Header Functional Descriptor
struct CDC_Header_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint16_t bcdCDC;
};

// Call Management Functional Descriptor
struct CDC_CM_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
    uint8_t bDataInterface;
};

// Abstract Control Management Functional Descriptor
struct CDC_ACM_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
};

// Direct Line Management Functional Descriptor
struct CDC_DLM_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
};

// Telephone Ringer Functional Descriptor
struct CDC_TR_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bRingerVolSteps;
    uint8_t bNumRingerPattern;
};

// Telephone Operational Modes Functional Descriptor
struct CDC_TOM_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
};

// Telephone Call and Line State Reporting Capabilities Descriptor
struct CDC_TCLSR_Capabilities_Desctriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint32_t bmCapabilities;
};

// Union Functional Descriptor
#define NUM_SLAVE_INTERFACES 1
struct CDC_Union_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bMasterInterface;
    uint8_t bSlaveInterface[NUM_SLAVE_INTERFACES];
};

// Country Selection Functional Descriptor
#define NUM_COUNTRY_CODES 1
struct CDC_CS_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t iCountryCodeRelDate;
    uint16_t wCountryCode[NUM_COUNTRY_CODES];
};

// USB Terminal Functional Descriptor
struct CDC_USB_Terminal_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t bInInterfaceNo;
    uint8_t bOutInterfaceNo;
    uint8_t bmOptions;
    uint8_t bChildId[1];
};

// Network Channel Terminal Functional Descriptor
struct CDC_NCT_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t iName;
    uint8_t bChannelIndex;
    uint8_t bPhysicalIndex;
};

// Protocol Unit Functional Descriptor
struct CDC_PU_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t bProtocol;
    uint8_t bChildId[1];
};

// Extension Unit Functional Descriptor
struct CDC_EU_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bEntityId;
    uint8_t bExtentionCode;
    uint8_t iName;
    uint8_t bChildId[1];
};

// Multi-Channel Management Functional Descriptor
struct CDC_MCM_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
};

// CAPI Control Management Functional Descriptor
struct CDC_CAPI_CM_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
};

// Ethernet Networking Functional Descriptor
struct CDC_EN_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t iMACAddress;
    uint32_t bmEthernetStatistics;
    uint16_t wMaxSegmentSize;
    uint16_t wNumberMCFilters;
    uint8_t bNumberPowerFilters;
};

// ATM Networking Functional Descriptor
struct CDC_ATM_Networking_Functional_Descriptor_Type{
    uint8_t bFunctionLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t iEndSystemIdentifier;
    uint8_t bmDataCapabilities;
    uint8_t bmATMDeviceStatistics;
    uint16_t wType2MaxSegmentSize;
    uint16_t wType3MaxSegmentSize;
    uint16_t wMaxVC;
};

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

struct{
    unsigned :8;
    unsigned :8;
    uint16_t Zero;
    uint16_t Interface;
    uint16_t Size_of_Structure;
}SetGetLineCodingData @SETUP_DATA_ADDR;

struct{
    uint32_t dwDTERate;
    uint8_t bCharFormat;
    uint8_t bParityType;
    uint8_t bDataBits;
}GetLineCodingReturnedData;

struct{
    uint32_t dwDTERate;
    uint8_t bCharFormat;
    uint8_t bParityType;
    uint8_t bDataBits;
}SetLineCodingData;

struct{
    unsigned :8;
    unsigned :8;
    unsigned DTR_bit :1;
    unsigned RTS_bit :1;
    unsigned :6;
    unsigned :8;
    uint8_t Zero;
}SetControlLineStateData @SETUP_DATA_ADDR;

// Class-Specific Notification Codes
#define NETWORK_CONNECTION      0x00
#define RESPONSE_AVAILABLE      0x01
#define AUX_JACK_HOOK_STATE     0x08
#define RING_DETECT             0x09
#define SERIAL_STATE            0x20
#define CALL_STATE_CHANGE       0x28
#define LINE_STATE_CHANGE       0x29
#define CONNECTION_SPEED_CHANGE 0x2A


#if defined(USE_DTR)||defined(USE_RTS)
typedef union {
    uint8_t Array[8];
    struct{
        uint8_t bmRequestType;
        uint8_t bNotification;
        uint16_t wValue;
        uint16_t wIndex;
        uint16_t wLength;
    };
}NotificationHeader_Type;

union{
    uint8_t Array[10];
    struct{
        NotificationHeader_Type Header;
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
}SERIAL_STATE_Data @CDC_COM_EP_IN_BUFFER_BASE_ADDR;
#endif

bool CDC_ClassRequest(void);
void CDC_Init(void);
void CDC_ClearEndpointToggle(void);
bool CDC_OutControlTasks(void);
void CDC_SetLineCoding(void);
void CDC_SetControlLineState(void);
void CDC_Tasks(void);
void CDC_DataOUT(void);
void CDC_DataIN(void);
void CDC_Notification(void);

void CDC_Arm_COM_EP_IN(void);
void CDC_Arm_DAT_EP_OUT(void);
void CDC_Arm_DAT_EP_IN(uint16_t cnt);

volatile bool setLineCodingWait;
volatile uint8_t NumDataOut;


#if defined(USE_DTR)||defined(USE_DCD)
void NotificationTasks(void);
bool sentLastNotification = true;
bool sendNotification = false;
#endif
#ifdef USE_RTS
bool USB_HAS_SET_RTS = false;
#endif
#endif