/**
 * @file usb_hid.c
 * @brief Contains <i>Human Interface Device Class</i> functions.
 * @author John Izzard
 * @date 17/12/2023
 * 
 * USB uC - HID Library.
 * Copyright (C) 2017-2023  John Izzard
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
#include <stdint.h>
#include <stdbool.h>
#include "usb.h"
#include "usb_hid.h"
#include "usb_ch9.h"

/* ************************************************************************** */
/* ***************************** REPORT TYPES ******************************* */
/* ************************************************************************** */

#define REPORT_INPUT   1
#define REPORT_OUTPUT  2
#define REPORT_FEATURE 3

/* ************************************************************************** */


/* ************************************************************************** */
/* ***************************** HID REQUESTS ******************************* */
/* ************************************************************************** */
enum
{
    GET_REPORT = 1,
    GET_IDLE,
    GET_PROTOCOL,
    SET_REPORT = 9,
    SET_IDLE,
    SET_PROTOCOL
};

/* ************************************************************************** */


/* ************************************************************************** */
/* ******************************** TYPES *********************************** */
/* ************************************************************************** */

typedef struct
{
    unsigned :8;
    unsigned :8;
    uint8_t  Report_ID;
    uint8_t  Report_Type;
    uint16_t Interface;
    uint16_t Report_Length;
}hid_get_set_report_t;

typedef struct
{
    unsigned :8;
    unsigned :8;
    uint8_t  Report_ID;
    unsigned :8;
    uint16_t Interface;
    uint16_t wLength;
}hid_get_idle_t;

typedef struct
{
    unsigned :8;
    unsigned :8;
    uint8_t  Report_ID;
    uint8_t  Duration;
    uint16_t Interface;
}hid_set_idle_t;

/* ************************************************************************** */


/* ************************************************************************** */
/* ***************************** HID ENDPOINTS ****************************** */
/* ************************************************************************** */

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_0_OUT
uint8_t g_hid_ep_out[HID_EP_SIZE] __at(HID_EP_OUT_BUFFER_BASE_ADDR);
uint8_t g_hid_ep_in[HID_EP_SIZE]  __at(HID_EP_IN_BUFFER_BASE_ADDR);

#else // PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
uint8_t g_hid_ep_out_even[HID_EP_SIZE]  __at(HID_EP_OUT_EVEN_BUFFER_BASE_ADDR);
uint8_t g_hid_ep_out_odd[HID_EP_SIZE]   __at(HID_EP_OUT_ODD_BUFFER_BASE_ADDR);
uint8_t g_hid_ep_in_even[HID_EP_SIZE]   __at(HID_EP_IN_EVEN_BUFFER_BASE_ADDR);
uint8_t g_hid_ep_in_odd[HID_EP_SIZE]    __at(HID_EP_IN_ODD_BUFFER_BASE_ADDR);
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* ****************************** GLOBAL VARS ******************************* */
/* ************************************************************************** */

volatile bool                    g_hid_report_sent = true;
volatile uint8_t                 g_hid_report_num_sent;
volatile bool                    g_hid_sent_report[HID_NUM_IN_REPORTS] = {true};
volatile hid_in_report_setting_t g_hid_in_report_settings[HID_NUM_IN_REPORTS];

/* ************************************************************************** */


/* ************************************************************************** */
/* **************** HID DESCRIPTORS FROM: usb_descriptors.c ***************** */
/* ************************************************************************** */

extern const uint8_t* g_hid_descriptor;
extern const uint8_t  g_hid_report_descriptor[];
extern const uint16_t g_hid_report_descriptor_size;

/* ************************************************************************** */


/* ************************************************************************** */
/* ******************* HID REPORTS FROM: usb_hid_reports.c ****************** */
/* ************************************************************************** */

extern const uint16_t g_hid_in_reports[];
extern const uint8_t  g_hid_in_report_size[];
extern const uint16_t g_hid_out_reports[];
extern const uint8_t  g_hid_out_report_size[];

/* ************************************************************************** */


/* ************************************************************************** */
/* ****************************** LOCAL VARS ******************************** */
/* ************************************************************************** */

static hid_get_set_report_t m_get_set_report __at(SETUP_DATA_ADDR);
static hid_get_idle_t       m_get_idle       __at(SETUP_DATA_ADDR);
static hid_set_idle_t       m_set_idle       __at(SETUP_DATA_ADDR);

/* ************************************************************************** */


/* ************************************************************************** */
/* ********************** LOCAL FUNCTION DECLARATIONS *********************** */
/* ************************************************************************** */

/**
 * @fn bool get_report(void)
 * 
 * @brief Returns Report on EP0.
 * 
 * The Get_Report request allows the host to receive a report via the Control pipe
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
static bool get_report(void);

/**
 * @fn bool set_report(void)
 * 
 * @brief Modifies Report on EP0.
 * 
 * The Set_Report request allows the host to send a report to the device, 
 * possibly setting the state of input, output, or feature controls.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
static bool set_report(void);

/**
 * @fn bool get_idle(void)
 * 
 * @brief Returns the Idle Duration and Idle Count.
 * 
 * The Get_Idle request reads the current idle rate for a particular Input report
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
static bool get_idle(void);

/**
 * @fn bool set_idle(void)
 * 
 * @brief Modifies the Idle Duration and Idle Count.
 * 
 * The Set_Idle request silences a particular report on the Interrupt In pipe 
 * until a new event occurs or the specified amount of time passes.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
static bool set_idle(void);

/* ************************************************************************** */


/* ************************************************************************** */
/* **************************** HID FUNCTIONS ******************************* */
/* ************************************************************************** */

#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
void hid_arm_ep_out(uint8_t bdt_index)
{
    usb_arm_endpoint(&g_usb_bd_table[bdt_index], &g_usb_ep_stat[HID_EP][OUT], HID_EP_SIZE);
}


void hid_arm_ep_in(uint8_t bdt_index, uint8_t cnt)
{
    usb_arm_endpoint(&g_usb_bd_table[bdt_index], &g_usb_ep_stat[HID_EP][IN], cnt);
}

#else
void hid_arm_ep_out(void)
{
    usb_arm_endpoint(&g_usb_bd_table[HID_BD_OUT], &g_usb_ep_stat[HID_EP][OUT], HID_EP_SIZE);
}


void hid_arm_ep_in(uint8_t cnt)
{
    usb_arm_endpoint(&g_usb_bd_table[HID_BD_IN], &g_usb_ep_stat[HID_EP][IN], cnt);
}
#endif

bool hid_class_request(void)
{
    switch(g_usb_setup.bRequest)
    {
        case GET_REPORT:
            return get_report();
        #ifdef USE_SET_REPORT
        case SET_REPORT:
        #endif
            return set_report();
        #ifdef USE_GET_IDLE
        case GET_IDLE:
            return get_idle();
        #endif
        #ifdef USE_SET_IDLE
        case SET_IDLE:
            return set_idle();
        #endif
        #ifdef USE_GET_PROTOCOL
        case GET_PROTOCOL:
        #endif
        #ifdef USE_SET_PROTOCOL
        case SET_PROTOCOL:
        #endif
        default:
            return false;
    }
}

bool hid_get_class_descriptor(const uint8_t** descriptor, uint16_t* size)
{
    // Stupid compiler cuts off address at 8 bits!
    uint32_t address;
    
    switch(g_usb_get_descriptor.DescriptorType)
    {
        case HID_DESC:
            //*descriptor = g_hid_descriptor; <----
            address = (uint32_t)g_hid_descriptor;
            *descriptor = (const uint8_t*)address;
            *size = 9;
            return true;
        case HID_REPORT_DESC:
            //*descriptor = g_hid_report_descriptor; <----
            address = (uint32_t)g_hid_report_descriptor;
            *descriptor = (const uint8_t*)address;
            *size = g_hid_report_descriptor_size;
            return true;
        default:
            return false;
    }
}

void hid_init(void)
{
    #if HID_NUM_OUT_REPORTS != 0
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    usb_ram_set(0, g_hid_ep_out_even, 8);
    usb_ram_set(0, g_hid_ep_out_odd, 8);
    
    g_usb_bd_table[HID_BD_OUT_EVEN].STAT = 0;
    g_usb_bd_table[HID_BD_OUT_EVEN].ADR = HID_EP_OUT_EVEN_BUFFER_BASE_ADDR;
    g_usb_bd_table[HID_BD_OUT_ODD].STAT = 0;
    g_usb_bd_table[HID_BD_OUT_ODD].ADR = HID_EP_OUT_ODD_BUFFER_BASE_ADDR;
    #else
    usb_ram_set(0, g_hid_ep_out, 8);
    
    g_usb_bd_table[HID_BD_OUT].STAT = 0;
    g_usb_bd_table[HID_BD_OUT].ADR = HID_EP_OUT_BUFFER_BASE_ADDR;
    #endif
    #endif
    #if HID_NUM_IN_REPORTS != 0
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    usb_ram_set(0, g_hid_ep_in_even, 8);
    usb_ram_set(0, g_hid_ep_in_odd, 8);
    
    
    g_usb_bd_table[HID_BD_IN_EVEN].STAT = 0;
    g_usb_bd_table[HID_BD_IN_EVEN].ADR  = HID_EP_IN_EVEN_BUFFER_BASE_ADDR;
    g_usb_bd_table[HID_BD_IN_ODD].STAT  = 0;
    g_usb_bd_table[HID_BD_IN_ODD].ADR   = HID_EP_IN_ODD_BUFFER_BASE_ADDR;
    #else
    usb_ram_set(0, g_hid_ep_in, 8);
    
    g_usb_bd_table[HID_BD_IN].STAT = 0;
    g_usb_bd_table[HID_BD_IN].ADR = HID_EP_IN_BUFFER_BASE_ADDR;
    #endif
    #endif
    
    // EP Settings
    HID_UEPbits.EPHSHK   = 1;  // Handshaking enabled 
    HID_UEPbits.EPCONDIS = 0;  // Don't allow SETUP
    #if HID_NUM_OUT_REPORTS != 0
    HID_UEPbits.EPOUTEN  = 1;  // EP output enabled
    g_usb_ep_stat[HID_EP][OUT].Halt = 0;
    #endif
    #if HID_NUM_IN_REPORTS != 0
    HID_UEPbits.EPINEN  = 1;   // EP input enabled
    g_usb_ep_stat[HID_EP][IN].Halt = 0;
    #if HID_NUM_IN_REPORTS > 1
    for(uint8_t i = 0; i < HID_NUM_IN_REPORTS; i++)
    {
		g_hid_sent_report[i] = true;
        g_hid_in_report_settings[i].Idle_Count_Overflow = false;
        g_hid_in_report_settings[i].Idle_Count = 0;
        g_hid_in_report_settings[i].Idle_Duration_1ms = DEFAULT_IDLE;
        g_hid_in_report_settings[i].Idle_Duration_4ms = DEFAULT_IDLE / 4;
    }
    #else
    g_hid_sent_report[0] = true;
	g_hid_in_report_settings[0].Idle_Count_Overflow = false;
    g_hid_in_report_settings[0].Idle_Count = 0;
    g_hid_in_report_settings[0].Idle_Duration_1ms = DEFAULT_IDLE;
    g_hid_in_report_settings[0].Idle_Duration_4ms = DEFAULT_IDLE / 4;
    #endif
    #endif
    hid_clear_ep_toggle();

    #if HID_NUM_OUT_REPORTS != 0
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    hid_arm_ep_out(HID_BD_OUT_EVEN);
    #else
    hid_arm_ep_out();
    #endif
    #endif
    g_hid_report_sent = true;
}

void hid_tasks(void)
{
    if(TRANSACTION_DIR == IN)
    {
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        HID_EP_IN_LAST_PPB = PINGPONG_PARITY;
        #endif
        HID_EP_IN_DATA_TOGGLE_VAL ^= 1;
        hid_set_sent_report_flag();
    }
    else
    {
        #if HID_NUM_OUT_REPORTS == 1
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        HID_EP_OUT_LAST_PPB = PINGPONG_PARITY;
        #endif
        HID_EP_OUT_DATA_TOGGLE_VAL ^= 1;
        
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        if(HID_EP_OUT_LAST_PPB == ODD)
        {
            usb_ram_copy((uint8_t*)HID_EP_OUT_ODD_BUFFER_BASE_ADDR, (uint8_t*)g_hid_out_reports[0], g_hid_out_report_size[0]);
            hid_out(0);
        }
        else
        {
            usb_ram_copy((uint8_t*)HID_EP_OUT_EVEN_BUFFER_BASE_ADDR, (uint8_t*)g_hid_out_reports[0], g_hid_out_report_size[0]);
            hid_out(0);
        }
        #else
        usb_ram_copy((uint8_t*)HID_EP_OUT_BUFFER_BASE_ADDR, (uint8_t*)g_hid_out_reports[0], g_hid_out_report_size[0]);
        hid_out(0);
        #endif
        #elif HID_NUM_OUT_REPORTS > 1
        uint8_t report_num;
        
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        HID_EP_OUT_LAST_PPB = PINGPONG_PARITY;
        #endif
        HID_EP_OUT_DATA_TOGGLE_VAL ^= 1;
        
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        uint8_t* ep_buff_base_addr = (uint8_t*)HID_EP_OUT_EVEN_BUFFER_BASE_ADDR;

        if(HID_EP_OUT_LAST_PPB == ODD) ep_buff_base_addr = (uint8_t*)HID_EP_OUT_ODD_BUFFER_BASE_ADDR;

        report_num = *ep_buff_base_addr;
        usb_ram_copy(ep_buff_base_addr, (uint8_t*)g_hid_out_reports[report_num], g_hid_out_report_size[report_num]);
        hid_out(report_num);
        #else
        report_num = *(uint8_t*)HID_EP_OUT_BUFFER_BASE_ADDR;
        usb_ram_copy((uint8_t*)HID_EP_OUT_BUFFER_BASE_ADDR, (uint8_t*)g_hid_out_reports[report_num], g_hid_out_report_size[report_num]);
        hid_out(report_num);
        #endif
        #endif
    }
}

void hid_clear_halt(uint8_t bdt_index, uint8_t ep, uint8_t dir)
{
    g_usb_ep_stat[ep][dir].Data_Toggle_Val = 0;
    
    if(g_usb_ep_stat[ep][dir].Halt)
    {
        g_usb_ep_stat[ep][dir].Halt      = 0;
        g_usb_bd_table[bdt_index].STAT   = 0;
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        g_usb_bd_table[++bdt_index].STAT = 0;
        #endif
    }
    if(dir == IN) hid_set_sent_report_flag();
}

void hid_set_sent_report_flag(void)
{
    g_hid_report_sent = true;
    g_hid_sent_report[g_hid_report_num_sent] = true;
}

void hid_clear_ep_toggle(void)
{
    g_usb_ep_stat[HID_EP][OUT].Data_Toggle_Val = 0;
    g_usb_ep_stat[HID_EP][IN].Data_Toggle_Val  = 0;
}

/* ************************************************************************** */


/* ************************************************************************** */
/* **************************** LOCAL FUNCTIONS ***************************** */
/* ************************************************************************** */

static bool get_report(void)
{
    uint16_t bytes_available = 0;

    if(m_get_set_report.Report_Type == REPORT_INPUT)
    {
        #if HID_NUM_IN_REPORTS == 0
        return false;
        #else
        #if HID_NUM_REPORT_IDS == 0
        if(m_get_set_report.Report_ID != 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_in_reports[0]);
        bytes_available = g_hid_in_report_size[0];
        #else
        if(m_get_set_report.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(m_get_set_report.Report_ID == 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_in_reports[m_get_set_report.Report_ID - 1u]);
        bytes_available = g_hid_in_report_size[m_get_set_report.Report_ID - 1u];
        #endif
        #endif
    }
    else if(m_get_set_report.Report_Type == REPORT_FEATURE)
    {
        #if HID_NUM_FEATURE_REPORTS == 0
        return false;
        #else
        #if HID_NUM_REPORT_IDS == 0
        if(m_get_set_report.Report_ID != 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_feature_reports[0]);
        bytes_available = g_hid_feature_report_size[0];
        #else
        if(m_get_set_report.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(m_get_set_report.Report_ID == 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_feature_reports[m_get_set_report.Report_ID - 1u]);
        bytes_available = g_hid_feature_report_size[m_get_set_report.Report_ID - 1u];
        #endif
        #endif
    }
    else return false;
    #if HID_NUM_IN_REPORTS != 0 || HID_NUM_FEATURE_REPORTS != 0
    usb_setup_in_control_transfer(RAM, bytes_available, m_get_set_report.Report_Length);
    usb_in_control_transfer();
    usb_set_control_stage(DATA_IN_STAGE);
    return true;
    #endif
}

#ifdef USE_SET_REPORT
static bool set_report(void)
{
    uint16_t bytes_available = 0;

    if(m_get_set_report.Report_Type == REPORT_OUTPUT)
    {
        #if HID_NUM_OUT_REPORTS == 0
        return false;
        #else
        #if HID_NUM_REPORT_IDS == 0
        if(m_get_set_report.Report_ID != 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_out_reports[0]);
        bytes_available = g_hid_out_report_size[0];
        #else
        if(m_get_set_report.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(m_get_set_report.Report_ID == 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_out_reports[m_get_set_report.Report_ID - 1u]);
        bytes_available = g_hid_out_report_size[m_get_set_report.Report_ID - 1u];
        #endif
        #endif
    }
    else if(m_get_set_report.Report_Type == REPORT_FEATURE)
    {
        #if HID_NUM_FEATURE_REPORTS == 0
        return false;
        #else
        #if HID_NUM_REPORT_IDS == 0
        if(m_get_set_report.Report_ID != 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_feature_reports[0]);
        bytes_available = g_hid_feature_report_size[0];
        #else
        if(m_get_set_report.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(m_get_set_report.Report_ID == 0) return false;
        usb_set_ram_ptr((uint8_t*)g_hid_feature_reports[m_get_set_report.Report_ID - 1u]);
        bytes_available = g_hid_feature_report_size[m_get_set_report.Report_ID - 1u];
        #endif
        #endif
    }
    else return false;
    #if HID_NUM_OUT_REPORTS != 0 || HID_NUM_FEATURE_REPORTS != 0
    if(m_get_set_report.Report_Length > bytes_available) return false;
    usb_set_num_out_control_bytes(m_get_set_report.Report_Length);
    usb_set_control_stage(DATA_OUT_STAGE);
    return true;
    #endif
}
#endif

#ifdef USE_GET_IDLE
static bool get_idle(void)
{
    #if HID_NUM_IN_REPORTS == 0
    return false;
    #else
    #if HID_NUM_REPORT_IDS == 0
    if(m_get_idle.Report_ID != 0) return false;
    usb_set_ram_ptr((uint8_t*)&g_hid_in_report_settings[0].Idle_Duration_4ms);
    #else
    if(m_get_idle.Report_ID > HID_NUM_REPORT_IDS) return false;
    if(m_get_idle.Report_ID == 0) return false;
    usb_set_ram_ptr((uint8_t*)&g_hid_in_report_settings[m_get_idle.Report_ID-1u].Idle_Duration_4ms);
    #endif
    if(m_get_idle.wLength != 1) return false;

    usb_setup_in_control_transfer(RAM, 1, 1);
    
    #if PINGPONG_MODE == PINGPONG_ALL_EP
    EP0_IN_LAST_PPB ^= 1;
    usb_in_control_transfer();
    #else
    usb_in_control_transfer();
    #endif
    usb_set_control_stage(DATA_IN_STAGE);
    
    return true;
    #endif
}
#endif

#ifdef USE_SET_IDLE
static bool set_idle(void)
{
    #if HID_NUM_IN_REPORTS == 0
    return false;
    #else
    #if HID_NUM_REPORT_IDS == 0
    if(m_set_idle.Report_ID != 0) return false;
	g_hid_in_report_settings[0].Idle_Count_Overflow = false;
	g_hid_in_report_settings[0].Idle_Count = 0;
    g_hid_in_report_settings[0].Idle_Duration_4ms = m_set_idle.Duration;
    g_hid_in_report_settings[0].Idle_Duration_1ms = ((uint16_t)m_set_idle.Duration) << 2;
    #else
    if(m_set_idle.Report_ID > HID_NUM_REPORT_IDS) return false;
    if(m_set_idle.Report_ID == 0) // All idles are set
    {
        for(uint8_t i = 0; i < HID_NUM_IN_REPORTS; i++)
        {
			g_hid_in_report_settings[i].Idle_Count_Overflow = false;
			g_hid_in_report_settings[i].Idle_Count = 0;
            g_hid_in_report_settings[i].Idle_Duration_4ms = m_set_idle.Duration;
            g_hid_in_report_settings[i].Idle_Duration_1ms = ((uint16_t)m_set_idle.Duration) << 2;
        }
    }
    else
    {
		g_hid_in_report_settings[m_set_idle.Report_ID - 1u].Idle_Count_Overflow = false;
		g_hid_in_report_settings[m_set_idle.Report_ID - 1u].Idle_Count = 0;
        g_hid_in_report_settings[m_set_idle.Report_ID - 1u].Idle_Duration_4ms = m_set_idle.Duration;
        g_hid_in_report_settings[m_set_idle.Report_ID - 1u].Idle_Duration_1ms = ((uint16_t)m_set_idle.Duration) << 2;
    }
    #endif
    usb_set_control_stage(STATUS_IN_STAGE);
    usb_arm_in_status();
    return true;
    #endif
}
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* ************************** FUNCTIONS FOR MAIN **************************** */
/* ************************************************************************** */

void hid_send_report(uint8_t report_num)
{
    if(g_hid_report_sent)
    {
        #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        uint8_t* ep_buffer_base_addr = (uint8_t*)HID_EP_IN_ODD_BUFFER_BASE_ADDR;
        uint8_t bd_in = HID_BD_IN_ODD;

        if(HID_EP_IN_LAST_PPB == ODD)
        {
            ep_buffer_base_addr = (uint8_t*)HID_EP_IN_EVEN_BUFFER_BASE_ADDR;
            bd_in = HID_BD_IN_EVEN;
        }

        usb_ram_copy((uint8_t*)g_hid_in_reports[report_num], ep_buffer_base_addr, g_hid_in_report_size[report_num]);
        hid_arm_ep_in(bd_in, g_hid_in_report_size[report_num]);

        #else
        usb_ram_copy((uint8_t*)g_hid_in_reports[report_num], (uint8_t*)HID_EP_IN_BUFFER_BASE_ADDR, g_hid_in_report_size[report_num]);
        hid_arm_ep_in(g_hid_in_report_size[report_num]);
        #endif
		USB_INTERRUPT_ENABLE = 0;
        g_hid_in_report_settings[report_num].Idle_Count = 0;
		USB_INTERRUPT_ENABLE = 1;
		g_hid_sent_report[report_num] = false;
		g_hid_report_num_sent = report_num;
        g_hid_report_sent = false;
    }
}

void hid_service_sof(void)
{
    if(usb_get_state() == STATE_CONFIGURED)
    {
        #if HID_NUM_IN_REPORTS > 1
        uint8_t i;
        for(i = 0; i < HID_NUM_IN_REPORTS; i++)
        {
            if(g_hid_in_report_settings[i].Idle_Duration_1ms != 0) 
                g_hid_in_report_settings[i].Idle_Count++; // If the Idle_Duration for a report is not infinite (0), increment their counter.
        }
        
        for(i = 0; i < HID_NUM_IN_REPORTS; i++) // If an Idle_Counter is not infinite and goes past the Idle_Duration, send the report.
        {
            if(g_hid_in_report_settings[i].Idle_Duration_1ms != 0 && g_hid_in_report_settings[i].Idle_Count > g_hid_in_report_settings[i].Idle_Duration_1ms - 1) 
                g_hid_in_report_settings[i].Idle_Count_Overflow = true;
        }
        #else
		if(g_hid_in_report_settings[0].Idle_Duration_1ms != 0)
        {
			g_hid_in_report_settings[0].Idle_Count++; // If the Idle_Duration is not infinite (0), increment the counter.
			if(g_hid_in_report_settings[0].Idle_Count > g_hid_in_report_settings[0].Idle_Duration_1ms - 1) 
                g_hid_in_report_settings[0].Idle_Count_Overflow = true;
        }
        #endif
    }
}

/* ************************************************************************** */