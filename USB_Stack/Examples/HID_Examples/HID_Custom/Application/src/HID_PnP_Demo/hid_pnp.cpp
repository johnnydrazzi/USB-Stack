#include "hid_pnp.h"

#define REPORT_0                   0x00
#define COMMAND_TOGGLE_LED         0x80
#define COMMAND_GET_BUTTON_STATUS  0x81
#define COMMAND_READ_POTENTIOMETER 0x37

HID_PnP::HID_PnP(QObject *parent) : QObject(parent)
{
    isConnected = false;
    pushbuttonStatus = false;
    potentiometerValue = 0;
    toggleLeds = 0;

    device = NULL;

    // Setup first out report
    buf[0] = REPORT_0;
    buf[1] = COMMAND_READ_POTENTIOMETER;
    memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
    
    // Initialise timer for polling
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));

    // Start timer at 250ms intervals
    timer->start(250);
}

HID_PnP::~HID_PnP()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));
}

void HID_PnP::PollUSB()
{
    // Setup for out report
    buf[0] = REPORT_0;
    memset((void*)&buf[2], 0x00, sizeof(buf) - 2);

    // Check if connected
    if(isConnected == false)
    {
        // If not connected, attempt to connect
        device = hid_open(0x04d8, 0x003f, NULL);

        if(device)
        {
            // Device detected, poll at 15ms interval
            isConnected = true;
            hid_set_nonblocking(device, true);
            timer->start(15);
        }
    }
    else
    {
        // Device is connected
        if(toggleLeds == true)
        {
            // Send COMMAND_TOGGLE_LED in out report
            toggleLeds = false;

            buf[1] = COMMAND_TOGGLE_LED;

            if(hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }

            // Next out report has COMMAND_READ_POTENTIOMETER
            buf[0] = REPORT_0;
            buf[1] = COMMAND_READ_POTENTIOMETER;
            memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
        }

        // Send next out report (either COMMAND_READ_POTENTIOMETER or COMMAND_GET_BUTTON_STATUS)
        if(hid_write(device, buf, sizeof(buf)) == -1)
        {
            CloseDevice();
            return;
        }

        // Read in report (response)
        if(hid_read(device, buf, sizeof(buf)) == -1)
        {
            CloseDevice();
            return;
        }

        // Process response and prepare other command
        if(buf[0] == COMMAND_READ_POTENTIOMETER)
        {
            potentiometerValue = (buf[2]<<8) + buf[1];
            buf[1] = COMMAND_GET_BUTTON_STATUS;
        }
        else if(buf[0] == COMMAND_GET_BUTTON_STATUS)
        {
            pushbuttonStatus = (buf[1] == 0x00);
            buf[1] = COMMAND_READ_POTENTIOMETER;
        }
    }

    emit hid_comm_update(isConnected, pushbuttonStatus, potentiometerValue);
}

void HID_PnP::toggle_leds()
{
    toggleLeds = true;
}

void HID_PnP::CloseDevice()
{
    hid_close(device);
    device = NULL;
    isConnected = false;
    pushbuttonStatus = false;
    potentiometerValue = 0;
    toggleLeds = 0;
    emit hid_comm_update(isConnected, pushbuttonStatus, potentiometerValue);
    timer->start(250);
}
