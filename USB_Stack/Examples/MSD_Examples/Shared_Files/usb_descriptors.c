/**
 * @file usb_descriptors.h
 * @brief Contains core USB stack descriptors stored in ROM.
 * @author John Izzard
 * @date 2024-11-14
 * 
 * USB uC - USB Stack (This file is for the the MSD Examples).
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

#include "usb_msd.h"
#include "usb_ch9.h"

/** Device Descriptor */
const ch9_device_descriptor_t g_device_descriptor =
{
    0x12,           // bLength:8 -  Size of descriptor in bytes
    DEVICE_DESC,    // bDescriptorType:8  - Device descriptor type
    0x0200,         // bcdUSB:16 -  USB in BCD (2.0H)
    0x00,           // bDeviceClass:8
    0x00,           // bDeviceSubClass:8
    0x00,           // bDeviceProtocol:8
    EP0_SIZE,       // bMaxPacketSize0:8 - Maximum packet size
    0x04D8,         // idVendor:16 - Microchip VID = 0x04D8
    0x00009,        // idProduct:16 - Product ID (VID) = 0x0009
    0x0001,         // bcdDevice:16 - Device release number in BCD
    0x01,           // iManufacturer:8 - Manufacturer string index
    0x02,           // iProduct:8 - Product string index
    0x03,           // iSerialNumber:8 - Device serial number string index
    0x01            // bNumConfigurations:8 - Number of possible configurations
};

/** Configuration Descriptor Structure */
typedef struct
{
    ch9_configutarion_descriptor_t      configuration0_descriptor;
    ch9_standard_interface_descriptor_t interface0_descriptor;
    ch9_standard_endpoint_descriptor_t  ep1_in_descriptor;
    ch9_standard_endpoint_descriptor_t  ep1_out_descriptor;
}config_descriptor_t;

/** Configuration Descriptor */
static const config_descriptor_t config_descriptor0 =
{
    // Configuration Descriptor
    {
        9,                          // bLength:8 - Size of configuration descriptor in bytes
        CONFIGURATION_DESC,         // bDescriptorType:8 - Configuration descriptor type
        sizeof(config_descriptor0), // wTotalLength:16 - Total amount of bytes in descriptors belonging to this configuration
        0x01,                       // bNumInterfaces:8 - Number of interfaces in this configuration
        0x01,                       // bConfigurationValue:8 - Index value for this configuration
        0x00,                       // iConfiguration:8 - Index of string describing this configuration
        0xC0,                       // bmAttributes:8 {0:5,RemoteWakeup:1,SelfPowered:1,1:1}
        50                          // bMaxPower:8 - 100mA power allowed (increments of 2mA)
    },

    // Interface Descriptor
    {
        9,              // bLength:8 - Size of interface descriptor in bytes           
        INTERFACE_DESC, // bDescriptorType:8 - Interface descriptor type
        0x00,           // bInterfaceNumber:8 - Index number of interface
        0x00,           // bAlternateSetting:8 - Value used to select alternate setting
        0x02,           // bNumEndpoints:8 - Number of endpoints used in this interface
        MSC,            // bInterfaceClass:8 - Class Code (Assigned by USB Org)
        SCSI_TRANSPARENT,// bInterfaceSubClass:8 - Subclass Code (Assigned by USB Org)
        BBB,            // bInterfaceProtocol:8 - Protocol Code (Assigned by USB Org)
        0x00            // iInterface:8 - Index of String Descriptor Describing this interface
    },

    // Endpoint Descriptor
    {
        7,                  // bLength:8 - Size of EP descriptor in bytes
        ENDPOINT_DESC,      // bDescriptorType:8 - Endpoint Descriptor Type
        0x81,               // bEndpointAddress:8 {EndpointNum:4,0:3,Direction:1} - Endpoint address
        0x02,               // bmAttributes:8 {TransferType:2,SyncType:2,UsageType:2,0:2} - Attributes
        EP1_SIZE,           // wMaxPacketSize:16 - Maximum packet size for this endpoint (send & receive)
        0x01                // bInterval:8 - Interval   //0x01
    },

    // Endpoint Descriptor
    {
        7,                // bLength:8 - Size of EP descriptor in bytes
        ENDPOINT_DESC,    // bDescriptorType:8 - Endpoint Descriptor Type
        0x01,             // bEndpointAddress:8 {EndpointNum:4,0:3,Direction:1} - Endpoint address
        0x02,             // bmAttributes:8 {TransferType:2,SyncType:2,UsageType:2,0:2} - Attributes
        EP1_SIZE,         // wMaxPacketSize:16 - Maximum packet size for this endpoint (send & receive)
        0x01              // bInterval:8 - Interval
    }
};

/** Configuration Descriptor Addresses Array */
const uint16_t g_config_descriptors[] =
{
    (uint16_t)&config_descriptor0
};

/** String Zero Descriptor Structure */
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wLANGID[1];
}string_zero_descriptor_t;

/** Vendor String Descriptor Structure */
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bString[25];
}vendor_string_descriptor_t;

/** Product String Descriptor Structure */
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bString[28];
}product_string_descriptor_t;

/** Serial String Descriptor Structure */
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bString[12];
}serial_string_descriptor_t;

/** String Zero Descriptor */
static const string_zero_descriptor_t string_zero_descriptor =
{
    sizeof(string_zero_descriptor_t),
    STRING_DESC,
    {0x0409}
};

/** Vendor String Descriptor */
static const vendor_string_descriptor_t vendor_string_descriptor =
{
    sizeof(vendor_string_descriptor_t),
    STRING_DESC,
    {'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}
};

/** Product String Descriptor */
static const product_string_descriptor_t product_string_descriptor =
{
    sizeof(product_string_descriptor_t),
    STRING_DESC,
    {'M','i','c','r','o','c','h','i','p',' ','M','a','s','s',' ','S','t','o','r','a','g','e',' ','D','r','i','v','e'}
};

/** Serial String Descriptor */
static const serial_string_descriptor_t serial_string_descriptor =
{
    sizeof(serial_string_descriptor_t),
    STRING_DESC,
    {'1','2','3','4','5','6','7','8','9','0','9','9'}
};

/** String Descriptor Addresses Array */
const uint16_t g_string_descriptors[] =
{
    (uint16_t)&string_zero_descriptor,
    (uint16_t)&vendor_string_descriptor,
    (uint16_t)&product_string_descriptor,
    (uint16_t)&serial_string_descriptor
};

/** String Descriptor Addresses Array Size */
const uint8_t g_size_of_sd = sizeof(g_string_descriptors);