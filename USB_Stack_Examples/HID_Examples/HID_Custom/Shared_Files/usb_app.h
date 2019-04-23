/**
 * @file usb_app.h
 * @brief Contains USB application related declarations of global prototypes and variables.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - USB Application file.
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
#ifndef USB_APP_H
#define USB_APP_H

#include <stdbool.h>
#include <stdint.h>

bool USB_ServiceClassRequest(void);
bool USB_GetClassDescriptor(void);
void USB_InitApp(void);
void USB_AppClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir);
void USB_AppTasks(void);
bool USB_AppSetInterface(uint8_t AlternateSetting, uint8_t Interface);
bool USB_AppGetInterface(uint8_t* AlternateSettingResult, uint8_t Interface);

#endif /* USB_APP_H */