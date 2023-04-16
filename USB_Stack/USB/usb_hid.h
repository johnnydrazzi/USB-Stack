/**
 * @file usb_hid.h
 * @brief <i>Human Interface Device Class</i> settings, structures, global variable and function declarations.
 * @author John Izzard
 * @date 16/04/2023
 * 
 * USB uC - HID Library (For Keyboard Examples).
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
#ifndef USB_HID_H
#define USB_HID_H

#include <stdint.h>
#include <stdbool.h>
#include "usb_config.h"
#include "usb_hal.h"
#include "usb.h"
#include "usb_hid_config.h"

/* ************************************************************************** */
/* ************************ HID ENDPOINT ADDRESSES ************************** */
/* ************************************************************************** */

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_1_15
#define HID_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE * 2))
#elif PINGPONG_MODE == PINGPONG_0_OUT
#define HID_EP_BUFFERS_STARTING_ADDR  (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE * 3))
#else
#define HID_EP_BUFFERS_STARTING_ADDR (EP_BUFFERS_STARTING_ADDR + (EP0_SIZE * 4))
#endif

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_0_OUT
#ifdef _PIC14E
#define HID_EP_OUT_BUFFER_BASE_ADDR 0x2050
#define HID_EP_IN_BUFFER_BASE_ADDR  0x20A0
#else
#define HID_EP_OUT_BUFFER_BASE_ADDR   HID_EP_BUFFERS_STARTING_ADDR
#define HID_EP_IN_BUFFER_BASE_ADDR   (HID_EP_BUFFERS_STARTING_ADDR + HID_EP_SIZE)
#endif

#else // PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
#ifdef _PIC14E
#define HID_EP_OUT_EVEN_BUFFER_BASE_ADDR 0x2050
#define HID_EP_OUT_ODD_BUFFER_BASE_ADDR  0x20A0
#define HID_EP_IN_EVEN_BUFFER_BASE_ADDR  0x20F0
#define HID_EP_IN_ODD_BUFFER_BASE_ADDR   0x2140
#else
#define HID_EP_OUT_EVEN_BUFFER_BASE_ADDR  HID_EP_BUFFERS_STARTING_ADDR
#define HID_EP_OUT_ODD_BUFFER_BASE_ADDR   (HID_EP_BUFFERS_STARTING_ADDR + HID_EP_SIZE)
#define HID_EP_IN_EVEN_BUFFER_BASE_ADDR   (HID_EP_BUFFERS_STARTING_ADDR + (HID_EP_SIZE * 2))
#define HID_EP_IN_ODD_BUFFER_BASE_ADDR    (HID_EP_BUFFERS_STARTING_ADDR + (HID_EP_SIZE * 3))
#endif
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* ******************************* HID CODES ******************************** */
/* ************************************************************************** */

/* DEFINES/STRUCTS FOR USB DESCRIPTORS */
// HID Subclass Codes
#define HID_BOOT 1

// HID Protocol Codes
#define HID_KEYBOARD 1
#define HID_MOUSE    2

#define HID_DESC          0x21
#define HID_REPORT_DESC   0x22
#define HID_PHYSICAL_DESC 0x23

/* ************************************************************************** */


/* ************************************************************************** */
/* ****************************** HID EP STAT ******************************* */
/* ************************************************************************** */

#define HID_EP_OUT_LAST_PPB        g_usb_ep_stat[HID_EP][OUT].Last_PPB
#define HID_EP_IN_LAST_PPB         g_usb_ep_stat[HID_EP][IN].Last_PPB
#define HID_EP_OUT_DATA_TOGGLE_VAL g_usb_ep_stat[HID_EP][OUT].Data_Toggle_Val
#define HID_EP_IN_DATA_TOGGLE_VAL  g_usb_ep_stat[HID_EP][IN].Data_Toggle_Val

/* ************************************************************************** */


/* ************************************************************************** */
/* ********************************* TYPES ********************************** */
/* ************************************************************************** */

typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdHID;
    uint8_t  bCountryCode;
    uint8_t  bNumDescriptors;
    uint8_t  bClassDescriptorType;
    uint16_t wDescriptorLength;
}hid_descriptor_t;

typedef struct 
{
    uint8_t  Idle_Duration_4ms;
    uint16_t Idle_Duration_1ms;
    uint16_t Idle_Count;
	bool     Idle_Count_Overflow;
}hid_in_report_setting_t;

/* ************************************************************************** */


/* ************************************************************************** */
/* ************************** VARS FROM: usb_hid_c ************************** */
/* ************************************************************************** */

#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_0_OUT
extern uint8_t g_hid_ep_out[HID_EP_SIZE] __at(HID_EP_OUT_BUFFER_BASE_ADDR);
extern uint8_t g_hid_ep_in[HID_EP_SIZE]  __at(HID_EP_IN_BUFFER_BASE_ADDR);

#else // PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
extern uint8_t g_hid_ep_out_even[HID_EP_SIZE]  __at(HID_EP_OUT_EVEN_BUFFER_BASE_ADDR);
extern uint8_t g_hid_ep_out_odd[HID_EP_SIZE]   __at(HID_EP_OUT_ODD_BUFFER_BASE_ADDR);
extern uint8_t g_hid_ep_in_even[HID_EP_SIZE]   __at(HID_EP_IN_EVEN_BUFFER_BASE_ADDR);
extern uint8_t g_hid_ep_in_odd[HID_EP_SIZE]    __at(HID_EP_IN_ODD_BUFFER_BASE_ADDR);
#endif

extern volatile bool                    g_hid_report_sent;
extern volatile uint8_t                 g_hid_report_num_sent;
extern volatile bool                    g_hid_sent_report[HID_NUM_IN_REPORTS];
extern volatile hid_in_report_setting_t g_hid_in_report_settings[HID_NUM_IN_REPORTS];

/* ************************************************************************** */


/* ************************************************************************** */
/* ***************************** HID FUNCTIONS ****************************** */
/* ************************************************************************** */

/**
 * @fn bool hid_class_request(void)
 * 
 * @brief Used to service Class Requests for HID.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool hid_class_request(void);

/**
 * @fn bool hid_get_class_descriptor(void)
 * 
 * @brief Used to service Get Class Descriptor Requests for HID.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool hid_get_class_descriptor(void);

/**
 * @fn void hid_init(void)
 * 
 * @brief Used to initialize EPs used by the HID library.
 * 
 * EP buffers are cleared, EP settings are configured, and the Buffer Descriptor Table values are setup.
 */
void hid_init(void);

/**
 * @fn void hid_tasks(void)
 * 
 * @brief Services HID tasks.
 */
void hid_tasks(void);

/**
 * @fn void hid_clear_halt(uint8_t bdt_index, uint8_t ep, uint8_t dir)
 * 
 * @brief Clear Endpoints used by HID.
 * 
 * Used to inform the HID library to clear a halt/stall condition on
 * it's endpoint.
 */
void hid_clear_halt(uint8_t bdt_index, uint8_t ep, uint8_t dir);

/**
 * @fn void hid_set_sent_report_flag(void)
 * 
 * @brief Lets the application know that a report has been sent.
 */
void hid_set_sent_report_flag(void);

/**
 * @fn void hid_clear_ep_toggle(void)
 * 
 * @brief Used to clear HID EP's Data Toggle values.
 */
void hid_clear_ep_toggle(void);

#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
/**
 * @fn hid_arm_ep_out(uint8_t bdt_index)
 * 
 * @brief Arms HID EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm HID EP OUT for a transaction.
 * 
 * @param[in] bdt_index Buffer Descriptor Index.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * hid_arm_ep_out(HID_BD_OUT_EVEN);
 * @endcode
 * </li></ul>
 */
void hid_arm_ep_out(uint8_t bdt_index);

/**
 * @fn void hid_arm_ep_in(uint8_t bdt_index, uint16_t cnt)
 * 
 * @brief Arms HID EP IN Endpoint for a transaction.
 * 
 * The function is used to arm HID EP IN for a transaction.
 * 
 * @param[in] bdt_index Buffer Descriptor Index.
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * hid_arm_ep_in(HID_BD_IN_EVEN,HID_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void hid_arm_ep_in(uint8_t bdt_index, uint16_t cnt);
#else

/**
 * @fn hid_arm_ep_out(void)
 * 
 * @brief Arms HID EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm HID EP OUT for a transaction.
 * 
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * hid_arm_ep_out();
 * @endcode
 * </li></ul>
 */
void hid_arm_ep_out(void);

/**
 * @fn void hid_arm_ep_in(uint16_t cnt)
 * 
 * @brief Arms HID EP IN Endpoint for a transaction.
 * 
 * The function is used to arm HID EP IN for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * hid_arm_ep_in(HID_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void hid_arm_ep_in(uint16_t cnt);
#endif

/* ************************************************************************** */


/* ************************************************************************** */
/* ************************** FUNCTIONS FOR MAIN **************************** */
/* ************************************************************************** */

/**
 * @fn void hid_send_report(uint8_t report_num)
 * 
 * @brief Sends OUT Report on HID EP.
 * 
 * @param report_num Report number to send.
 */
void hid_send_report(uint8_t report_num);

/**
 * @fn void hid_service_sof(void)
 * 
 * @brief Sends OUT Report on HID EP regularly using SOF.
 */
void hid_service_sof(void);

/** @fn void hid_out(void)
 * 
 * @brief Function to run in main, so you can respond to reception of 
 * OUT Reports.
 * 
 * @param report_num Report number received.
 */
void hid_out(uint8_t report_num);

/* ************************************************************************** */

#endif /* USB_HID_H */