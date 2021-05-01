# USB-Stack
![Alt text](Images/Image.jpg?raw=true "USB_Stack") 
A PIC16 and PIC18 Device USB Stack. Supports all pingpong buffering types. Support for endpoints 0-7.

**Inlcuded are examples:**
- CDC Serial
- CDC Serial UART
- HID Custom
- HID Keyboard + Consumer
- HID Mouse
- HID RubberDucky
- MSD Internal
- MSD Simple

**Currently supports:**
- PIC16F1454
- PIC16F1455
- PIC16F1459
- PIC18F13K50
- PIC18F14K50
- PIC18F24K50
- PIC18F25K50
- PIC18F45K50
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


