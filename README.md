# USB-Stack
![Alt text](Images/Image.jpg?raw=true "USB_Stack") 
A PIC16 and PIC18 Device USB Stack. Supports all pingpong buffering types. Support for endpoints 0-7.

The USB stack has been compliance tested using USB-IF's USB 3 Command Verifier (USB3CV) tool.

**Inlcuded are examples:**
- CDC Serial
- CDC Serial UART
- HID Custom
- HID Keyboard + Consumer
- HID Mouse
- HID RubberDucky
- MSD Internal
- MSD Simple

**Currently supports:**<br>
PIC16F1459 Family:
- PIC16F1454
- PIC16F1455
- PIC16F1459

PIC18F4450 Family:
- PIC18F2450
- PIC18F4450

PIC18F4550 Family:
- PIC18F2455
- PIC18F4455
- PIC18F2550
- PIC18F4550

PIC18F4553 Family:
- PIC18F2458
- PIC18F4458
- PIC18F2553
- PIC18F4553

PIC18F1XK50 Family:
- PIC18F13K50
- PIC18F14K50

PIC18F45K50 Family:
- PIC18F24K50
- PIC18F25K50
- PIC18F45K50

PIC18F46J50 Family:
- PIC18F24J50
- PIC18F44J50
- PIC18F25J50
- PIC18F45J50
- PIC18F26J50
- PIC18F46J50

PIC18F47J53 Family:
- PIC18F26J53
- PIC18F46J53
- PIC18F27J53
- PIC18F47J53

**Files you modify to suit your project:**
- config.h
- main.c
- usb_config.h
- usb_app.c
- usb_descriptors.c
- usb_cdc_config.h
- usb_hid_config.h
- usb_hid_reports.h
- usb_hid_reports.c
- usb_msd_config.h
- usb_scsi_inq.c

**Getting Started:**
1. Choose an example to open.
2. Uncomment the chosen example in usb_config.h (will configure usb and class config files for the chosen example).
3. Select your target microcontroller from the configurations dropdown list.
4. Select your development board from the config.h file.
5. Add bootloader settings to project (optional)(see in main.c file for instructions).

For more information visit the [Project Page](https://hackaday.io/project/165152-pic16-pic18-usb-stack) on Hackaday.io.


