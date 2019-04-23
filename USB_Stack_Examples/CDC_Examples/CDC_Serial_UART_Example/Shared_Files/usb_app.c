/**
 * @file usb_app.c
 * @brief Contains application specific functions.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - USB Application file (This file is for MSD Simple Example).
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
#include <stdbool.h>
#include "USB.h"
#include "usb_app.h"
#include "usb_cdc.h"

/**
 * @fn bool USB_ServiceClassRequest(void)
 * 
 * @brief Used to service Class Requests on EP0.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool USB_ServiceClassRequest(void){
    return CDC_ClassRequest();
}

/**
 * @fn bool USB_GetClassDescriptor(void)
 * 
 * @brief Used to send Class Descriptors.
 * 
 * @return Returns true if descriptor is to be sent.
 */
bool USB_GetClassDescriptor(void){
    return false;
}

/**
 * @fn void USB_InitApp(void)
 * 
 * @brief Used to initialize class libraries.
 */
void USB_InitApp(void){
    CDC_Init();
}

/**
 * @fn void USB_AppTasks(void)
 * 
 * @brief Services the Application.
 * 
 * The device state machine calls this whenever an endpoint other than EP0 is used.
 */
void USB_AppTasks(void){
    CDC_Tasks();
}

/**
 * @fn void USB_AppClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir)
 * 
 * @brief Clear Endpoints used by the Application.
 * 
 * Used to inform the Application to clear the halt/stall condition on one of
 * it's endpoints.
 */
void USB_AppClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir){
    EPn_Stat.Array[EP][dir].Halt = 0;
    EPn_Stat.Array[EP][dir].Data_Toggle_Val = 0;
    BDT.Array[BDT_Index].STAT = 0;
}

/**
 * @fn bool USB_AppSetInterface(uint8_t AlternateSetting, uint8_t Interface)
 * 
 * @brief Services a SetInterface Request.
 * 
 * Services a SetInterface Request.
 * 
 * @param AlternateSetting Alternate Setting to change to.
 * @param Interface Interface the request is directed at.
 * @return Returns true if successful and false if unsuccessful.
 */
bool USB_AppSetInterface(uint8_t AlternateSetting, uint8_t Interface){
#if NUM_ALT_INTERFACES != 0
    if(SetInterfaceData.Interface<NUM_INTERFACES) return false;
#else
    if(AlternateSetting == 0 && Interface == 0){
        CDC_COM_EP_IN_DATA_TOGGLE_VAL = 0;
        return true;
    }
    else if(AlternateSetting == 0 && Interface == 1){
        CDC_DAT_EP_OUT_DATA_TOGGLE_VAL = 0;
        CDC_DAT_EP_IN_DATA_TOGGLE_VAL = 0;
        return true;
    }
    else return false;
#endif
}

/**
 * @fn bool USB_AppGetInterface(uint8_t* AlternateSettingResult, uint8_t Interface)
 * 
 * @brief Services a GetInterface Request.
 * 
 * Returns the Alternate Setting used for the specified Interface.
 * 
 * @param AlternateSettingResult Alternate Setting return.
 * @param Interface Interface the request is directed at.
 * @return Returns true if successful and false if unsuccessful.
 */
bool USB_AppGetInterface(uint8_t* AlternateSettingResult, uint8_t Interface){
#if NUM_ALT_INTERFACES != 0
    if(GetInterfaceData.Interface<NUM_INTERFACES) return false;
#endif
    return false;
}

/**
 * @fn bool USB_OutControlFinished(void)
 * 
 * @brief Is called at the end of a OUT Control Transfer.
 * 
 * @return Returns true if successful and false if unsuccessful.
 */
bool USB_OutControlFinished(void){
    return CDC_OutControlTasks();
}