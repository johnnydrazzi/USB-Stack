#include "hid_pnp.h"

#define COMMAND_TOGGLE_LED         0x80
#define COMMAND_GET_BUTTON_STATUS  0x81
#define COMMAND_READ_POTENTIOMETER 0x37

HID_PnP::HID_PnP(QObject *parent) : QObject(parent) {
    isConnected = false;
    pushbuttonStatus = false;
    potentiometerValue = 0;
    toggleLeds = 0;

    device = nullptr;
    memset(buf, 0x00, sizeof(buf));
    buf[0] = 0x00;
    buf[1] = COMMAND_READ_POTENTIOMETER;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));

    timer->start(250);
}

HID_PnP::~HID_PnP() {
    disconnect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));
}

void HID_PnP::PollUSB()
{
    buf[0] = 0x00;
    memset(&buf[2], 0x00, sizeof(buf) - 2);

    if (isConnected == false) {
        device = hid_open(0x04d8, 0x003f, nullptr);

        if (device) {
            isConnected = true;
            hid_set_nonblocking(device, true);
            timer->start(15); // When device is connected poll every 15ms.
        }
    }
    else {
        if(toggleLeds == true) {
            toggleLeds = false;

            buf[1] = COMMAND_TOGGLE_LED;

            if (hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }

            memset(buf, 0x00, sizeof(buf));
            buf[1] = COMMAND_READ_POTENTIOMETER;
        }

        if (hid_write(device, buf, sizeof(buf)) == -1)
        {
            CloseDevice();
            return;
        }
        if(hid_read(device, buf, sizeof(buf)) == -1)
        {
            CloseDevice();
            return;
        }
        if(buf[0] == COMMAND_READ_POTENTIOMETER) {
            potentiometerValue = (buf[2]<<8) + buf[1];
            buf[1] = COMMAND_GET_BUTTON_STATUS;
        }
        else if(buf[0] == COMMAND_GET_BUTTON_STATUS) {
            pushbuttonStatus = (buf[1] == 0x00);
            buf[1] = COMMAND_READ_POTENTIOMETER;
        }
    }

    hid_comm_update(isConnected, pushbuttonStatus, potentiometerValue);
}

void HID_PnP::toggle_leds() {
    toggleLeds = true;
}

void HID_PnP::CloseDevice() {
    hid_close(device);
    device = nullptr;
    isConnected = false;
    pushbuttonStatus = false;
    potentiometerValue = 0;
    toggleLeds = false;
    hid_comm_update(isConnected, pushbuttonStatus, potentiometerValue);
    timer->start(250); // When device is disconnected poll every 250ms.
}
