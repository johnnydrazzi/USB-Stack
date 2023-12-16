/**
 * @file usb_cdc_acm.c
 * @brief <i>Communications Device Class</i> core.
 * @author John Izzard
 * @date 30/04/2023
 * 
 * USB uC - CDC Library.
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

#include "usb.h"
#include "usb_cdc.h"
#include "usb_hal.h"
#include "usb_ch9.h"

/* ************************************************************************** */
/* **************************** CDC ENDPOINTS ******************************* */
/* ************************************************************************** */

uint8_t g_cdc_com_ep_in[CDC_COM_EP_SIZE]  __at(CDC_COM_EP_IN_BUFFER_BASE_ADDR);
uint8_t g_cdc_dat_ep_out[CDC_DAT_EP_SIZE] __at(CDC_DAT_EP_OUT_BUFFER_BASE_ADDR);
uint8_t g_cdc_dat_ep_in[CDC_DAT_EP_SIZE]  __at(CDC_DAT_EP_IN_BUFFER_BASE_ADDR);

/* ************************************************************************** */


/* ************************************************************************** */
/* ***************************** GLOBAL VARS ******************************** */
/* ************************************************************************** */

cdc_set_get_line_coding_t    g_cdc_set_get_line_coding     __at(SETUP_DATA_ADDR);
cdc_set_control_line_state_t g_cdc_set_control_line_state  __at(SETUP_DATA_ADDR);
cdc_get_line_coding_return_t g_cdc_get_line_coding_return;
cdc_set_line_coding_t        g_cdc_set_line_coding;

#if defined(USE_DTR) || defined(USE_RTS)
cdc_serial_state_t           g_cdc_serial_state            __at(CDC_COM_EP_IN_BUFFER_BASE_ADDR);
#endif

volatile bool    g_cdc_set_line_coding_wait;
volatile uint8_t g_cdc_num_data_out;

#if defined(USE_DTR) || defined(USE_DCD)
bool g_cdc_sent_last_notification = true;
bool g_cdc_send_notification      = false;
#endif
#ifdef USE_RTS
bool g_cdc_has_set_rts = false;
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* *************************** CDC FUNCTIONS ******************************** */
/* ************************************************************************** */

void cdc_arm_com_ep_in(void)
{
    usb_arm_endpoint(&g_usb_bd_table[CDC_COM_BD_IN], &g_usb_ep_stat[CDC_COM_EP][IN], 10);
}


void cdc_arm_data_ep_out(void)
{
    usb_arm_endpoint(&g_usb_bd_table[CDC_DAT_BD_OUT], &g_usb_ep_stat[CDC_DAT_EP][OUT], CDC_DAT_EP_SIZE);
}


void cdc_arm_data_ep_in(uint8_t cnt)
{
    usb_arm_endpoint(&g_usb_bd_table[CDC_DAT_BD_IN], &g_usb_ep_stat[CDC_DAT_EP][IN], cnt);
}

bool cdc_class_request(void)
{
    static uint8_t dummy_buffer[8] = {0};
    
    switch(g_usb_setup.bRequest)
    {
        #ifdef USE_GET_LINE_CODING
        case GET_LINE_CODING:
            g_usb_ram_ptr = (uint8_t*)&g_cdc_get_line_coding_return;
            g_usb_bytes_available = 7;
            if(g_usb_bytes_available < g_cdc_set_get_line_coding.Size_of_Structure)
            {
                g_usb_bytes_2_send = g_usb_bytes_available;
                g_usb_send_short   = g_cdc_set_get_line_coding.Size_of_Structure % g_usb_bytes_available ? true : false;
            }
            else
            {
                g_usb_bytes_2_send = g_cdc_set_get_line_coding.Size_of_Structure;
                g_usb_send_short   = false;
            }
            g_usb_sending_from = RAM;
            usb_in_control_transfer();
            usb_set_control_stage(DATA_IN_STAGE);
            return true;
        #endif
        #ifdef USE_SET_LINE_CODING
        case SET_LINE_CODING:
            g_usb_ram_ptr = (uint8_t*)&g_cdc_set_line_coding;
            if(g_cdc_set_get_line_coding.Size_of_Structure > 7) return false;
            g_usb_bytes_2_recv = g_cdc_set_get_line_coding.Size_of_Structure;
            g_cdc_set_line_coding_wait = true;
            usb_set_control_stage(DATA_OUT_STAGE);
            return true;
        #endif
        #ifdef USE_SET_CONTROL_LINE_STATE
        case SET_CONTROL_LINE_STATE:
            if(g_usb_setup.wIndex != CDC_COM_INT) return false;
            cdc_set_control_line_state();
            usb_arm_in_status();
            return true;
        #endif
        case SEND_ENCAPSULATED_COMMAND:
            g_usb_ram_ptr = dummy_buffer;
            if(g_usb_setup.wLength > 8) return false;
            g_usb_bytes_2_recv = g_usb_setup.wLength;
            usb_set_control_stage(DATA_OUT_STAGE);
            return true;
        case GET_ENCAPSULATED_RESPONSE:
            g_usb_ram_ptr = dummy_buffer;
            g_usb_bytes_available = 8;
            if(g_usb_bytes_available < g_usb_setup.wLength)
            {
                g_usb_bytes_2_send = g_usb_bytes_available;
                g_usb_send_short   = g_usb_setup.wLength % g_usb_bytes_available ? true : false;
            }
            else
            {
                g_usb_bytes_2_send = g_usb_setup.wLength;
                g_usb_send_short   = false;
            }
            g_usb_sending_from = RAM;
            usb_in_control_transfer();
            usb_set_control_stage(DATA_IN_STAGE);
            return true;
        default:
            return false;
    }
    

}

void cdc_init(void)
{
    #ifdef USE_GET_LINE_CODING
    g_cdc_get_line_coding_return.dwDTERate   = STARTING_BAUD;
    g_cdc_get_line_coding_return.bCharFormat = STARTING_STOP_BITS;
    g_cdc_get_line_coding_return.bParityType = PARITY_NONE;
    g_cdc_get_line_coding_return.bDataBits   = STARTING_DATA_BITS;
    #endif
    
    #ifdef USE_RTS
    RTS = RTS_ACTIVE ^ 1;
    RTS_TRIS = 0;
    #endif
    #ifdef USE_DTR
    DTR = DTR_ACTIVE ^ 1;
    DTR_TRIS = 0;
    #endif
    
    #if defined(USE_DTR) || defined(USE_DCD)
    usb_ram_set(0, g_cdc_serial_state.array, 10);
    g_cdc_serial_state.header.bmRequestType = 0xA1;
    g_cdc_serial_state.header.bNotification = SERIAL_STATE;
    g_cdc_serial_state.header.wValue  = 0;
    g_cdc_serial_stateg_serial_state.header.wIndex  = 1;
    g_cdc_serial_state.header.wLength = 2;
    #ifdef USE_DCD
    g_cdc_serial_state.bRxCarrier = (DCD ^ DCD_ACTIVE) ^ 1; // DCD
    #else
    g_cdc_serial_stateg_serial_state.bRxCarrier = 1; // DCD
    #endif
    #ifdef USE_DTR
    g_cdc_serial_state.bTxCarrier = (DSR ^ DSR_ACTIVE) ^ 1; // DSR
    #else
    g_cdc_serial_state.bTxCarrier = 1; // DSR
    #endif
    #endif

    // BD settings
    g_usb_bd_table[CDC_COM_BD_IN].STAT  = 0;
    g_usb_bd_table[CDC_COM_BD_IN].ADR   = CDC_COM_EP_IN_BUFFER_BASE_ADDR;
    g_usb_bd_table[CDC_DAT_BD_OUT].STAT = 0;
    g_usb_bd_table[CDC_DAT_BD_OUT].ADR  = CDC_DAT_EP_OUT_BUFFER_BASE_ADDR;
    g_usb_bd_table[CDC_DAT_BD_IN].STAT  = 0;
    g_usb_bd_table[CDC_DAT_BD_IN].ADR   = CDC_DAT_EP_IN_BUFFER_BASE_ADDR;
    
    // EP Settings
    CDC_COM_UEPbits.EPHSHK   = 1; // Handshaking enabled 
    CDC_COM_UEPbits.EPINEN   = 1; // EP input enabled
    CDC_DAT_UEPbits.EPHSHK   = 1; // Handshaking enabled 
    CDC_DAT_UEPbits.EPCONDIS = 0; // Don't allow SETUP
    CDC_DAT_UEPbits.EPOUTEN  = 1; // EP output enabled
    CDC_DAT_UEPbits.EPINEN   = 1; // EP input enabled
    
    g_usb_ep_stat[CDC_COM_EP][IN].Halt  = 0;
    g_usb_ep_stat[CDC_DAT_EP][OUT].Halt = 0;
    g_usb_ep_stat[CDC_DAT_EP][IN].Halt  = 0;
    cdc_clear_ep_toggle();
    cdc_arm_data_ep_out();
    
    #if defined(USE_DTR) || defined(USE_DCD)
    cdc_arm_com_ep_in();
    #endif
    g_cdc_set_line_coding_wait = false;
}

void cdc_clear_ep_toggle(void)
{
    CDC_COM_EP_IN_DATA_TOGGLE_VAL  = 0;
    CDC_DAT_EP_OUT_DATA_TOGGLE_VAL = 0;
    CDC_DAT_EP_IN_DATA_TOGGLE_VAL  = 0;
}

void cdc_tasks(void)
{
    if(TRANSACTION_EP == CDC_COM_EP)
    {
        CDC_COM_EP_IN_DATA_TOGGLE_VAL ^= 1;
        cdc_notification();
    }
    else if(TRANSACTION_EP == CDC_DAT_EP)
    {
        if(TRANSACTION_DIR == OUT)
        {
            CDC_DAT_EP_OUT_DATA_TOGGLE_VAL ^= 1;
            g_cdc_num_data_out = g_usb_bd_table[CDC_DAT_BD_OUT].CNT;
            cdc_data_out();
        }
        else
        {
            CDC_DAT_EP_IN_DATA_TOGGLE_VAL ^= 1;
            cdc_data_in();
        }
    }
}

bool cdc_out_control_tasks(void)
{
    #ifdef USE_SET_LINE_CODING
    if(g_cdc_set_line_coding_wait)
    {
        g_cdc_set_line_coding_wait = false;
        if(g_cdc_set_line_coding.bCharFormat != 0) return false;
        if(g_cdc_set_line_coding.bParityType != 0) return false;
        if(g_cdc_set_line_coding.bDataBits   != 8) return false;
        
        g_cdc_get_line_coding_return.dwDTERate   = g_cdc_set_line_coding.dwDTERate;
        g_cdc_get_line_coding_return.bCharFormat = g_cdc_set_line_coding.bCharFormat;
        g_cdc_get_line_coding_return.bParityType = g_cdc_set_line_coding.bParityType;
        g_cdc_get_line_coding_return.bDataBits   = g_cdc_set_line_coding.bDataBits;
        
        cdc_set_line_coding();
        return true;
    }
    #endif
    return false;
}

#if defined(USE_DTR) || defined(USE_DCD)
void cdc_notification_tasks(void)
{
    if(g_cdc_sent_last_notification && g_cdc_send_notification)
    {
        g_cdc_send_notification      = false;
        g_cdc_sent_last_notification = false;
        cdc_arm_com_ep_in();
    }
}
#endif

/* ************************************************************************** */