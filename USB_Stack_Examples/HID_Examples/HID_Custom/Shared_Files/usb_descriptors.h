/**
 * @file usb_descriptors.h
 * @brief Contains core USB stack descriptors stored in ROM.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - USB Stack (This file is for HID Keyboard + Consumer Example).
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
#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H
#include "usb_hid.h"

const struct Device_Descriptor_Type Device_Descriptor = {
    0x12,           // bLength:8 -  Size of descriptor in bytes
    DEVICE_DESC,    // bDescriptorType:8  - Device descriptor type
    0x0200,         // bcdUSB:16 -  USB in BCD (2.0H)
    0x00,           // bDeviceClass:8
    0x00,           // bDeviceSubClass:8
    0x00,           // bDeviceProtocol:8
    EP0_SIZE,       // bMaxPacketSize0:8 - Maximum packet size
    0x04D8,         // idVendor:16 - Microchip VID = 0x04D8
    0x003F,         // idProduct:16 - Product ID (VID) = 0x0055
    0x0100,         // bcdDevice:16 - Device release number in BCD
    0x01,           // iManufacturer:8 - Manufacturer string index
    0x02,           // iProduct:8 - Product string index
    0x00,           // iSerialNumber:8 - Device serial number string index
    0x01            // bNumConfigurations:8 - Number of possible configurations
};

struct Config_Descriptor_Type {
    struct Configutarion_Descriptor_Type Configuration0_Descriptor;
    struct Standard_Interface_Descriptor_Type Interface0_Descriptor;
    struct HID_Descriptor_Type HID_Descriptor;
    struct Standard_Endpoint_Descriptor_Type EP1_IN_Descriptor;
    struct Standard_Endpoint_Descriptor_Type EP1_OUT_Descriptor;
};

const struct Config_Descriptor_Type Config_Descriptor0 = {
    // Configuration Descriptor
    {
        9,                          // bLength:8 - Size of configuration descriptor in bytes
        CONFIGURATION_DESC,         // bDescriptorType:8 - Configuration descriptor type
        sizeof(Config_Descriptor0), // wTotalLength:16 - Total amount of bytes in descriptors belonging to this configuration    //0x34
        0x01,                       // bNumInterfaces:8 - Number of interfaces in this configuration
        0x01,                       // bConfigurationValue:8 - Index value for this configuration
        0x00,                       // iConfiguration:8 - Index of string describing this configuration
        0xC0,                       // bmAttributes:8 {0:5,RemoteWakeup:1,SelfPowered:1,1:1} - Self Powered
        50                          // bMaxPower:8 - 100mA power allowed (increments of 2mA)
    },

    // Interface Descriptor
    {
        9,              // bLength:8 - Size of interface descriptor in bytes           
        INTERFACE_DESC, // bDescriptorType:8 - Interface descriptor type
        0x00,           // bInterfaceNumber:8 - Index number of interface
        0x00,           // bAlternateSetting:8 - Value used to select alternate setting
        0x02,           // bNumEndpoints:8 - Number of endpoints used in this interface
        HID,            // bInterfaceClass:8 - Class Code (Assigned by USB Org)
        0,              // bInterfaceSubClass:8 - Subclass Code (Assigned by USB Org)
        0,              // bInterfaceProtocol:8 - Protocol Code (Assigned by USB Org)
        0x00            // iInterface:8 - Index of String Descriptor Describing this interface
    },

    // HID Descriptor
    {
        9,                  // bLength:8 - Size of HID descriptor in bytes 
        HID_DESC,           // bDescriptorType:8 - HID descriptor type
        0x0111,             // bcdHID:16 - HID spec release (1.11)
        0x00,               // bCountryCode:8 - Country code of localized hardware
        HID_NUM_DESC,       // bNumDescriptors:8 - Number of HID class descriptors 
        HID_REPORT_DESC,    // bClassDescriptorType:8 - HID class descriptor type
        sizeof(HID_Report_Descriptor) // wDescriptorLength:16 - Size of HID report descriptor in bytes
    },

    // Endpoint Descriptor
    {
        7,                  // bLength:8 - Size of EP descriptor in bytes
        ENDPOINT_DESC,      // bDescriptorType:8 - Endpoint Descriptor Type
        0x81,               // bEndpointAddress:8 {EndpointNum:4,0:3,Direction:1} - Endpoint address
        0x03,               // bmAttributes:8 {TransferType:2,SyncType:2,UsageType:2,0:2} - Attributes
        EP1_SIZE,           // wMaxPacketSize:16 - Maximum packet size for this endpoint (send & receive)
        0x01                // bInterval:8 - Interval
    },

    // Endpoint Descriptor
    {
        7,                // bLength:8 - Size of EP descriptor in bytes
        ENDPOINT_DESC,    // bDescriptorType:8 - Endpoint Descriptor Type
        0x01,             // bEndpointAddress:8 {EndpointNum:4,0:3,Direction:1} - Endpoint address
        0x03,             // bmAttributes:8 {TransferType:2,SyncType:2,UsageType:2,0:2} - Attributes
        EP1_SIZE,         // wMaxPacketSize:16 - Maximum packet size for this endpoint (send & receive)
        0x01              // bInterval:8 - Interval
    }
};

/** Configuration Descriptor Addresses Array */
const uint16_t CD_Ptr[] = {
    (uint16_t)&Config_Descriptor0
};

struct String_Zero_Descriptor_Type {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t  wLANGID[1];
};

struct Vendor_String_Descriptor_Type {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t  bString[6];
};

struct Product_String_Descriptor_Type {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t  bString[23];
};
    
const struct String_Zero_Descriptor_Type String_Zero_Descriptor = {
    4,
    STRING_DESC,
    {0x0409}
};

const struct Vendor_String_Descriptor_Type Vendor_String_Descriptor = {
    sizeof(Vendor_String_Descriptor),
    STRING_DESC,
    {'J','o','h','n','n','y'}
};

const struct Product_String_Descriptor_Type Product_String_Descriptor = {
    sizeof(Product_String_Descriptor),
    STRING_DESC,
    {'S','i','m','p','l','e',' ','H','I','D',' ','D','e','v','i','c','e',' ','D','e','m','o'}
};

const uint16_t SD_Ptr[] = {
    (uint16_t)&String_Zero_Descriptor,
    (uint16_t)&Vendor_String_Descriptor,
    (uint16_t)&Product_String_Descriptor
};

#endif /* USB_DESCRIPTORS_H */