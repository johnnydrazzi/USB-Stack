/**
 * @file usb_ch9.h
 * @brief <i>USB Device Framework</i> structures and enums.
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

#ifndef USB_CH9_H
#define USB_CH9_H

#include <stdint.h>

/// SETUP DATA
union{
    uint8_t Array[8];
    struct{
        union{
            struct{
                unsigned Recipient              :5;
                unsigned Type                   :2;
                unsigned DataTransferDirection  :1;
            }bmRequestType_bits;
            uint8_t bmRequestType;
        };
        uint8_t bRequest;
        uint16_t wValue;
        uint16_t wIndex;
        uint16_t wLength;
    };
}SetupData @SETUP_DATA_ADDR;

/// bmRequestType D7: Data transfer direction
#define Host_to_device 0x00
#define Device_to_host 0x01

/// bmRequestType D6...5: Type
#define Standard 0x00
#define Class    0x01
#define Vendor   0x02

/// bmRequestType D4...0: Recipient
enum{
    Device = 0,
    Interface,
    Endpoint,
    Other
};

///  Standard Request Codes (bRequest)
#define GET_STATUS        0x00
#define CLEAR_FEATURE     0x01
#define SET_FEATURE       0x03
#define SET_ADDRESS       0x05
#define GET_DESCRIPTOR    0x06
#define SET_DESCRIPTOR    0x07
#define GET_CONFIGURATION 0x08
#define SET_CONFIGURATION 0x09
#define GET_INTERFACE     0x0A
#define SET_INTERFACE     0x0B
#define SYNC_FRAME        0x0C

/// SET/CLEAR FEATURE REQUEST DATA
struct{
    union{
        struct{
            unsigned Recipient:5;
            unsigned Type:2;
            unsigned Direction:1;
        }bmRequestType_bits;
        uint8_t bmRequestType;
    };
    unsigned :8;
    uint16_t FeatureSelector;
    union{
        uint16_t ZeroInterfaceEndpoint;
        union{
            uint16_t Zero;
            uint16_t Interface;
            uint16_t Endpoint;
            struct{
                uint8_t InterfaceNumber;
                uint8_t TestSelector;
            }Interface_bits;  
            struct{
                unsigned EndpointNumber         :4;
                unsigned Reserved_ALWAYS_CLEAR  :3;
                unsigned Direction              :1;
                uint8_t  TestSelector;
            }Endpoint_bits;
        }ZeroInterfaceEndpoint_bits;  
    };
    uint16_t Zero;
}SetClearFeatureData @SETUP_DATA_ADDR;

/// Standard Feature Selectors
enum{
    DEVICE_REMOTE_WAKEUP = 1,
    ENDPOINT_HALT = 0,
    TEST_MODE = 2
};

/// Test Mode Selectors
enum{
    Test_J = 1,
    Test_K,
    Test_SE0_NAK,
    Test_Packet,
    Test_Force_Enable
};

/// GET CONFIGURATION REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint32_t Zero;
    uint16_t One;
}GetConfigurationData @SETUP_DATA_ADDR;

/// GET DESCRIPTOR REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint8_t DescriptorIndex;
    uint8_t DescriptorType;
    uint16_t LanguageID_ZERO;
    uint16_t DescriptorLength;
}GetDescriptorData @SETUP_DATA_ADDR;

/// SET DESCRIPTOR REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint8_t DescriptorIndex;
    uint8_t DescriptorType;
    uint16_t LanguageID_ZERO;
    uint16_t DescriptorLength;
}SetDescriptorData @SETUP_DATA_ADDR;

/// Descriptor Types
enum{
    DEVICE_DESC = 1,
    CONFIGURATION_DESC,
    STRING_DESC,
    INTERFACE_DESC,
    ENDPOINT_DESC,
    DEVICE_QUALIFIER_DESC,
    OTHER_SPEED_CONFIGURATION_DESC,
    INTERFACE_POWER_DESC
};

/// Class Codes
#define NA  0
#define CDC 2
#define HID 3
#define MSC 8

/// GET INTERFACE REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint16_t Zero;
    uint8_t  Interface;
    unsigned Reserved_ALWAYS_CLEAR :8;
    uint16_t One;
}GetInterfaceData @SETUP_DATA_ADDR;

/// SET INTERFACE REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint16_t AlternateSetting;
    uint8_t  Interface;
    unsigned Reserved_ALWAYS_CLEAR :8;
    uint16_t Zero;
}SetInterfaceData @SETUP_DATA_ADDR;

/// GET STATUS REQUEST DATA
struct{
    union{
        struct{
            unsigned Recipient  :5;
            unsigned Type       :2;
            unsigned Direction  :1;
        }bmRequestType_bits;
        uint8_t bmRequestType;
    };
    unsigned :8;
    uint16_t Zero;
    union{
        uint16_t ZeroInterfaceEndpoint;
        union{
            uint16_t Zero;
            union{
                uint16_t Interface;
                struct{
                    uint8_t InterfaceNumber;
                    uint8_t Reserved_ALWAYS_CLEAR;
                }Interface_bits;
            };
            union{
                uint16_t Endpoint;
                struct{
                    unsigned EndpointNumber          :4;
                    unsigned Reserved0_ALWAYS_CLEAR  :3;
                    unsigned Direction               :1;
                    uint8_t  Reserved1_ALWAYS_CLEAR;
                }Endpoint_bits;
            };
            
        }ZeroInterfaceEndpoint_bits;  
    };
    uint16_t Two;
}GetStatusData @SETUP_DATA_ADDR;

/// SET ADDRESS REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint16_t DeviceAddress;
    uint32_t Zero;
}SetAddressData @SETUP_DATA_ADDR;

/// SET CONFIGURATION REQUEST DATA
struct{
    unsigned :8;
    unsigned :8;
    uint16_t ConfigurationValue;
    uint32_t Zero;
}SetConfigurationData @SETUP_DATA_ADDR;

/// SYNC FRAME REQUEST DATA
struct{
    unsigned                        :8;
    unsigned                        :8;
    uint16_t Zero;
    unsigned Endpoint               :4;
    unsigned Reserved_ALWAYS_CLEAR  :4;
    unsigned Direction              :1;
    uint16_t Two;
}SyncFrameData @SETUP_DATA_ADDR;

/******************************************************/
/******************************************************/

/// Device Descriptor Structure
struct Device_Descriptor_Type{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
};

/// Device Qualifier Descriptor Structure
struct Device_Qualifier_Descriptor_Type{ // *** ONLY USED IN HIGH SPEED
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint8_t  bNumConfigurations;
    uint8_t  bReserved;
};

/// Configuration Descriptor Structure
struct Configutarion_Descriptor_Type{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  bMaxPower;
};

/// Other Speed Configuration Descriptor Structure
struct Other_Speed_Configutarion_Descriptor_Type{ // *** ONLY USED IN HIGH SPEED
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  bMaxPower;
};

/// Standard Interface Descriptor Structure
struct Standard_Interface_Descriptor_Type{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bInterfaceNumber;
    uint8_t  bAlternateSetting;
    uint8_t  bNumEndpoints;
    uint8_t  bInterfaceClass;
    uint8_t  bInterfaceSubClass;
    uint8_t  bInterfaceProtocol;
    uint8_t  iInterface;
};

/// Standard Endpoint Descriptor Structure
struct Standard_Endpoint_Descriptor_Type{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
};

#endif /* USB_CH9_H */