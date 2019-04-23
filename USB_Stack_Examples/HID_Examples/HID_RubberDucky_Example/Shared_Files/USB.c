/**
 * @file USB.c
 * @brief Contains the USB stack core functions.
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

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "USB.h"
#include "usb_app.h"
#include "usb_ch9.h"
#include "usb_descriptors.h"

static uint8_t saved_address;
static bool UpdateAddress;

static void USB_ArmSetup(void);
static void USB_ProcessSetup(void);

static void USB_GET_STATUS(void);
static void USB_SET_CLEAR_FEATURE(void);
static void USB_SET_ADDRESS(void);
static void USB_GET_DESCRIPTOR(void);
static void USB_SET_DESCRIPTOR(void);
static void USB_GET_CONFIGURATION(void);
static void USB_SET_CONFIGURATION(void);
static void USB_GET_INTERFACE(void);
static void USB_SET_INTERFACE(void);
static void USB_SYNC_FRAME(void);
/** 
 * @fn void USB_Init(void)
 * 
 * @brief Sets up USB for operation.
 * 
 * Firstly resets USB peripheral to default values, clears out endpoints,
 * and sets variables starting values for first operation.
 */
void USB_Init(void){
    uint16_t i;
    
    USB_InterruptEnableRegister = INTERRUPTS_MASK;
    USB_ErrorInterruptEnableRegister = ERROR_INTERRUPT_MASK;
    
    USB_InterruptStatRegister  = 0;   // Interrupt flags cleared
    USB_ErrorInterruptStatRegister = 0;
    
    USB_EP0_ControlRegister = 0; // Disable EP0
    
    // Disable EPs > EP0 
#if NUM_ENDPOINTS > 1 
    USB_EP1_ControlRegister = _EPCONDIS;
#endif
#if NUM_ENDPOINTS > 2
    USB_EP2_ControlRegister = _EPCONDIS;
#endif
#if NUM_ENDPOINTS > 3
    USB_EP3_ControlRegister = _EPCONDIS;
#endif
#if NUM_ENDPOINTS > 4
    USB_EP4_ControlRegister = _EPCONDIS;
#endif
#if NUM_ENDPOINTS > 5
    USB_EP5_ControlRegister = _EPCONDIS;
#endif
#if NUM_ENDPOINTS > 6
    USB_EP6_ControlRegister = _EPCONDIS;
#endif
#if NUM_ENDPOINTS > 7
    USB_EP7_ControlRegister = _EPCONDIS;
#endif
    
    UADDR = 0x00;// Address starts off at 0x00
    
    // USB Settings
    DEVS.Self_Powered = POWERED_TYPE;
    DEVS.Remote_Wakeup = REMOTE_WAKEUP;
    USB_ConfigurationRegister = SPEED_PULLUP | USB_SPEED | PPB;
    
    // Clear BDT Entries
    for(i=0;i<BDT_SIZE;i++){
        BDT.Clear[i] = 0;
    }
    
    // Setup BDT for EP0 OUT
#if PINGPONG_MODE == PINGPONG_ALL_EP || PINGPONG_MODE == PINGPONG_0_OUT
    BDT.Array[BD0_OUT_EVEN].ADR = EP0_OUT_EVEN_BUFFER_BASE_ADDR;
    BDT.Array[BD0_OUT_ODD].ADR  = EP0_OUT_ODD_BUFFER_BASE_ADDR;
#else
    BDT.Array[BD0_OUT].ADR = EP0_OUT_BUFFER_BASE_ADDR;
#endif
    
    // Setup BDT for EP0 IN
#if PINGPONG_MODE == PINGPONG_ALL_EP
    BDT.Array[BD0_IN_EVEN].ADR = EP0_IN_EVEN_BUFFER_BASE_ADDR;
    BDT.Array[BD0_IN_ODD].ADR  = EP0_IN_ODD_BUFFER_BASE_ADDR;
#else
    BDT.Array[BD0_IN].ADR = EP0_IN_BUFFER_BASE_ADDR;
#endif
    
    // Clear EP statuses
    for(i=0;i<EPn_SIZE;i++){
        EPn_Stat.Clear[i] = 0;
    }
    
#if PINGPONG_MODE != PINGPONG_DIS
    // Next_PPB starts on ODD
    for(i=0;i<NUM_ENDPOINTS;i++){
        EPn_Stat.Array[i][0].Last_PPB = ODD;
        EPn_Stat.Array[i][1].Last_PPB = ODD;
    }
#endif
    
    UpdateAddress = false;
    send_short = false;
    
    CurrentConfiguration = 0;
    
    while(TransactionCompleteFlag){
        TransactionCompleteFlag = 0; // Clear USTAT
    }
    PacketTransferDisable = 0;
    
    // EP0 Settings for control
    USB_EP0_ControlRegister = _EPHSHK | _EPOUTEN | _EPINEN;
    
    if(USB_STATE == Detached){
        USB_ModuleEnable = 1;
        USB_STATE = Attached;
        while(SingleEndedZero){}
        USB_STATE = Powered;
    }
    
    ControlStage = SETUP_STAGE;
    
#if PINGPONG_MODE != PINGPONG_DIS
    PPB_Reset = 1; // Reset Ping-Pong Buffer Pointers to Even
    NOP();
    NOP();
    NOP();
    NOP();
    PPB_Reset = 0;
#endif
    
#if PINGPONG_MODE == PINGPONG_DIS || PINGPONG_MODE == PINGPONG_1_15
    USB_ArmSetup();
#else
    USB_ArmSetup();
    EPn_Stat.Array[EP0][OUT].Last_PPB = EVEN;
    USB_ArmSetup();
#endif
}
/** 
 * @fn void USB_Close(void)
 * 
 * @brief Detaches from USB bus and clears USB related registers.
 */
void USB_Close(void){
    
    USB_ControlRegister = 0;
    USB_ConfigurationRegister = 0;
    USB_EP0_ControlRegister = 0; // Disable EP0
    
    // Disable EPs > EP0 
#if NUM_ENDPOINTS > 1 
    USB_EP1_ControlRegister = 0;
#endif
#if NUM_ENDPOINTS > 2
    USB_EP2_ControlRegister = 0;
#endif
#if NUM_ENDPOINTS > 3
    USB_EP3_ControlRegister = 0;
#endif
#if NUM_ENDPOINTS > 4
    USB_EP4_ControlRegister = 0;
#endif
#if NUM_ENDPOINTS > 5
    USB_EP5_ControlRegister = 0;
#endif
#if NUM_ENDPOINTS > 6
    USB_EP6_ControlRegister = 0;
#endif
#if NUM_ENDPOINTS > 7
    USB_EP7_ControlRegister = 0;
#endif
    
    while(TransactionCompleteFlag){
        TransactionCompleteFlag = 0; // Clear USTAT
    }
    USB_InterruptEnableRegister  = 0; // Interrupts disabled
    USB_ErrorInterruptEnableRegister = 0;
    
    USB_InterruptStatRegister  = 0;   // Interrupt flags cleared
    USB_ErrorInterruptStatRegister = 0;
    
    USB_STATE = Detached;
}
/**
 * @fn void USB_Tasks(void)
 * 
 * @brief The core USB state machine for handling USB device state and tasks.
 * 
 * The <i>USB_Tasks()</i> function is designed to run frequently in the main program loop to handle all USB tasks when polling method is used and run in ISR when interrupts are enabled.
 * The function contains the USB Device state machine as seen in <i>Section 9.1.1</i> of the <i>USB Specification 2.0</i>.
 */
void USB_Tasks(void){
    static uint8_t USB_STATEprev;
    
    if(ActivityDetectFlag && ActivityDetectEnable){
#ifdef USE_ACTIVITY
        USB_Activity();
#endif
        ActivityDetectEnable = 0;
        UIEbits.ACTVIE = 0;
        
        if(USB_STATE == Suspended){
            USB_Suspend = 0;
            USB_STATE = USB_STATEprev;
        }
        
        while(ActivityDetectFlag) ActivityDetectFlag = 0;
    }
    if(USB_STATE == Suspended) return;
    if(ResetConditionFlag){
        if(USB_STATE != Powered) USB_Init();
        USB_STATE = Default;
#ifdef USE_RESET
        USB_Reset();
#endif
        ResetConditionFlag = 0;
    }
    if(IdleDetectFlag){
        ActivityDetectEnable = 1;
        UIEbits.ACTVIE = 1;
        USB_Suspend = 1;
        USB_STATEprev = USB_STATE;
        USB_STATE = Suspended;
#ifdef USE_IDLE
        USB_Idle();
#endif
        IdleDetectFlag = 0;
    }
#ifdef USE_SOF
    if(SOF_Flag){
        USB_SOF();
        SOF_Flag = 0;
    }
#endif
#ifdef USE_ERROR
    if(ErrorConditionFlag){
        USB_Error();
        ErrorConditionFlag = 0;
    }
#endif
    if(USB_STATE < Default) return;
    while(TransactionCompleteFlag){
        NOP();
        NOP();
        LastUSTAT.BYTE = USTAT;       // Save a copy of USTAT and clear the Transaction Complete Flag.
        TransactionCompleteFlag = 0;  // This is to advance the FIFO fast as possible.
        if(TRANSACTION_EP == EP0){
            if(TRANSACTION_DIR == OUT){
#if PINGPONG_MODE == PINGPONG_0_OUT || PINGPONG_MODE == PINGPONG_ALL_EP
                EP0_OUT_LAST_PPB = PINGPONG_PARITY;
                if(BDT.Array[PINGPONG_PARITY].STATbits.PID == PID_SETUP_TOKEN){
#else
                if(BDT.Array[BD0_OUT].STATbits.PID == PID_SETUP_TOKEN){
#endif
                    USB_ProcessSetup();
                }
                else{
                    if(ControlStage == DATA_OUT_STAGE){
                        EP0_OUT_DATA_TOGGLE_VAL ^= 1;
                        USB_OutControlTransfer();
                    }
                    USB_ArmSetup();
                }
            }
            else{
#if PINGPONG_MODE == PINGPONG_ALL_EP
                EP0_IN_LAST_PPB = PINGPONG_PARITY;
#endif
                if(ControlStage == DATA_IN_STAGE){
                    EP0_IN_DATA_TOGGLE_VAL ^= 1;
                    USB_InControlTransfer();
                }
                else{
                    USB_ArmSetup();
                    if(UpdateAddress){
                        UADDR = saved_address;
                        if(USB_STATE == Default && saved_address != 0) USB_STATE = Address;
                        else if(saved_address == 0) ResetConditionFlag = 1;
                        UpdateAddress = false;
                    }
                }
            }
        }
        else{
            USB_AppTasks();
        }
    }
}
/**
 * @fn void USB_ArmSetup(void)
 * 
 * @brief Arms the OUT Control Endpoint (EP0 OUT) for <i>Setup Transaction</i>.
 * 
 * The <i>USB_ArmSetup()</i> function sets up the EP0 OUT buffer descriptor values in preparation
 * for a <i>Setup Transaction</i>. The <i>Setup Packet</i> is 8 bytes long and will be stored
 * in the EP0_OUT buffer following the successful completion of the transaction. The setup
 * data can then accessed via the SetupData structure.
 */
static void USB_ArmSetup(void){
#if PINGPONG_MODE == PINGPONG_0_OUT || PINGPONG_MODE == PINGPONG_ALL_EP
    BDT.Array[EP0_OUT_LAST_PPB].CNT = 8;
    BDT.Array[EP0_OUT_LAST_PPB].STAT = 0;
    BDT.Array[EP0_OUT_LAST_PPB].STAT |= _UOWN;
#else
    BDT.Array[BD0_OUT].CNT = 8;
    BDT.Array[BD0_OUT].STAT = 0;
    BDT.Array[BD0_OUT].STAT |= _UOWN;
#endif
}
/**
 * @fn void USB_ArmEndpoint(BD* BD_ptr, EPS_Type* EPS_ptr, uint16_t buffer_addr, uint16_t cnt)
 * 
 * @brief Arms any OUT/IN Endpoint for a transaction.
 * 
 * The function is used to arm any endpoint (OUT/IN) for a transaction.
 * 
 * @param[in] BD_ptr Buffer Descriptor (BD) pointer.
 * @param[in] EPS_ptr Endpoint Settings (EPS_Type) pointer.
 * @param[in] buffer_addr Address of data in RAM to transfer in data stage.
 * @param[in] cnt Amount of data being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * USB_ArmEndpoint(&BDT.Array[BD1_OUT], &EPn_Stat.Array[EP1][OUT], EP1_OUT_BUFFER_BASE_ADDR, 31);
 * @endcode
 * </li></ul>
 */
void USB_ArmEndpoint(BD* BD_ptr, EPS_Type* EPS_ptr, uint16_t buffer_addr, uint16_t cnt){
    if(EPS_ptr->Data_Toggle_Val){
        BD_ptr->STAT = _DTSEN | _DTS; // DATA1
    }
    else{
        BD_ptr->STAT = _DTSEN;        // DATA0
    }
    BD_ptr->CNT = cnt;
    BD_ptr->ADR = buffer_addr;
    BD_ptr->STAT |= _UOWN;
}

#if PINGPONG_MODE == PINGPONG_ALL_EP
/**
 * @fn void USB_Arm_EP0_IN(uint8_t BDT_Index, uint16_t cnt)
 * 
 * @brief Arms EP0 IN Endpoint for a transaction.
 * 
 * The function is used to arm EP0 IN for a transaction.
 * 
 * @param[in] BDT_Index Buffer Descriptor Index.
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * USB_Arm_EP0_IN(BD0_IN_EVEN,8);
 * @endcode
 * </li></ul>
 */
void USB_Arm_EP0_IN(uint8_t BDT_Index, uint16_t cnt){
    if(EPn_Stat.Array[EP0][IN].Data_Toggle_Val) BDT.Array[BDT_Index].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[BDT_Index].STAT = _DTSEN; // DATA0
    BDT.Array[BDT_Index].CNT = cnt;
    BDT.Array[BDT_Index].STAT |= _UOWN;
}
#else
/**
 * @fn void USB_Arm_EP0_IN(uint16_t cnt)
 * 
 * @brief Arms EP0 IN Endpoint for a transaction.
 * 
 * The function is used to arm EP0 IN for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * USB_Arm_EP0_IN(8);
 * @endcode
 * </li></ul>
 */
void USB_Arm_EP0_IN(uint16_t cnt){
    if(EPn_Stat.Array[EP0][IN].Data_Toggle_Val) BDT.Array[BD0_IN].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[BD0_IN].STAT = _DTSEN; // DATA0
    BDT.Array[BD0_IN].CNT = cnt;
    BDT.Array[BD0_IN].STAT |= _UOWN;
}
#endif
/**
 * @fn void USB_ArmStatus(BD* BD_ptr)
 * 
 * @brief Arms any OUT/IN Endpoint for status stage (Zero-length data packet). 
 * 
 * @param[in] BD_ptr Buffer Descriptor (BD) pointer.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * USB_ArmStatus(&BDT.Array[BD0_IN]);
 * @endcode
 * </li></ul>
 */
void USB_ArmStatus(BD* BD_ptr){
    BD_ptr->CNT = 0;
    BD_ptr->STAT = _DTSEN | _DTS;
    BD_ptr->STAT |= _UOWN;
}
/**
 * @fn void USB_ArmINStatus(void)
 * 
 * @brief Arms any EP0 IN Endpoint for status stage (Zero-length data packet). 
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * USB_ArmINStatus();
 * @endcode
 * </li></ul>
 */
void USB_ArmINStatus(void){
#if PINGPONG_MODE == PINGPONG_ALL_EP
    if(EP0_IN_LAST_PPB == ODD) USB_ArmStatus(&BDT.Array[BD0_IN_EVEN]);
    else USB_ArmStatus(&BDT.Array[BD0_IN_ODD]);
#else
    USB_ArmStatus(&BDT.Array[BD0_IN]);
#endif
}
/**
 * @fn void USB_Stall_EP(BD* BD_ptr)
 * 
 * @brief Stalls any OUT/IN Endpoint. 
 * 
 * @param[in] BD_ptr Buffer Descriptor (BD) pointer.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * EPn_Stat.Array[EP1][IN].Halt = 1;
 * USB_Stall_EP(&BDT.Array[BD1_IN]);
 * @endcode
 * </li></ul>
 */
void USB_Stall_EP(BD* BD_ptr){
        BD_ptr->STAT = _BSTALL;
        BD_ptr->STAT |= _UOWN;
}
/**
 * @fn void USB_ProcessSetup(void)
 * 
 * @brief Processes the setup transaction.
 * 
 * The function parses the setup packet stored in SetupData and sets up for 
 * the execution of Standard and Class Requests.
 * 
 */
static void USB_ProcessSetup(void){
#if PINGPONG_MODE == PINGPONG_ALL_EP
    BDT.Array[BD0_IN_EVEN].STAT = 0; // Stops any pending control IN transfers.
    BDT.Array[BD0_IN_ODD].STAT = 0;  // Stops any pending control IN transfers.
#else
    BDT.Array[BD0_IN].STAT = 0; // Stops any pending control IN transfers.
#endif
    
#if PINGPONG_MODE == PINGPONG_0_OUT || PINGPONG_MODE == PINGPONG_ALL_EP
    if(PINGPONG_PARITY == ODD)  USB_RAMCopy(EP0_OUT_ODD, SetupData.Array, 8); // Store Setup Packet Data and straight-away re-arm EP0 OUT.
    else USB_RAMCopy(EP0_OUT_EVEN, SetupData.Array, 8);
#else
    USB_RAMCopy(EP0_OUT, SetupData.Array, 8); // Store Setup Packet Data and straight-away re-arm EP0 OUT.
#endif
    PacketTransferDisable = 0; // Must be cleared after every setup transfer!    
    USB_ArmSetup(); // USB device should be fast as possible to ACK a new setup packet, that's why we rearm straight-away.
                    // EP0 OUT is also armed to accept OUT Status packets.
    
    EP0_OUT_DATA_TOGGLE_VAL = 1; // First transfer after setup is always DATA1 type.
    EP0_IN_DATA_TOGGLE_VAL = 1;  // First transfer after setup is always DATA1 type.
    
    // Now process the SetupData.
    if(SetupData.bmRequestType_bits.Type == Standard){
        switch(SetupData.bRequest){
            // Most common requests are checked first for speed.
            case GET_DESCRIPTOR:
                USB_GET_DESCRIPTOR();
                break;
            case CLEAR_FEATURE:
                USB_SET_CLEAR_FEATURE();
                break;
            case SET_ADDRESS:
                USB_SET_ADDRESS();
                break;                
            case SET_CONFIGURATION:
                USB_SET_CONFIGURATION();
                break;
                
                
            case GET_STATUS:
                USB_GET_STATUS();
                break;
            case SET_FEATURE:
                USB_SET_CLEAR_FEATURE();
                break;
            case SET_DESCRIPTOR:
                USB_SET_DESCRIPTOR();
                break;
            case GET_CONFIGURATION:
                USB_GET_CONFIGURATION();
                break;
            case GET_INTERFACE:
                USB_GET_INTERFACE();
                break;
            case SET_INTERFACE:
                USB_SET_INTERFACE();
                break;
            case SYNC_FRAME:
                USB_SYNC_FRAME();
                break;
            default:
                USB_RequestError();
                break;
        }
    }
    else{
        if(USB_ServiceClassRequest() == false) {USB_RequestError();}
    }
}
/**
 * @fn void USB_InControlTransfer(void)
 * 
 * @brief Processes a IN Control (EP0 IN) transaction.
 * 
 */
void USB_InControlTransfer(void){
#if PINGPONG_MODE == PINGPONG_ALL_EP
    uint8_t *EPaddress;
    uint8_t BDT_Index;
    
    if(EP0_IN_LAST_PPB == ODD){
        EPaddress = EP0_IN_ODD;
        BDT_Index = BD0_IN_ODD;
    }
    else{
        EPaddress = EP0_IN_EVEN;
        BDT_Index = BD0_IN_EVEN;
    }
    
    if(bytes_2_send){
        if(bytes_2_send>EP0_SIZE){
            if(SendingFrom == ROM){
                USB_ROMCopy((const uint8_t*)((uint24_t)ROMptr),EPaddress,EP0_SIZE);
                ROMptr += EP0_SIZE;
            }
            else{
                USB_RAMCopy((uint8_t*)((uint24_t)RAMptr),EPaddress,EP0_SIZE);
                RAMptr += EP0_SIZE;
            }
            USB_Arm_EP0_IN(BDT_Index, EP0_SIZE);
            bytes_2_send -= EP0_SIZE;
        }
        else{
            if(SendingFrom == ROM) USB_ROMCopy((const uint8_t*)((uint24_t)ROMptr),EPaddress,bytes_2_send);
            else USB_RAMCopy((uint8_t*)((uint24_t)RAMptr),EPaddress,bytes_2_send);
            USB_Arm_EP0_IN(BDT_Index, bytes_2_send);
            bytes_2_send = 0;
        }
    }
    else{
        if(send_short){
            USB_Arm_EP0_IN(BDT_Index, 0);
            send_short = false;
        }
    }
#else
    if(bytes_2_send){
        if(bytes_2_send>EP0_SIZE){
            if(SendingFrom == ROM){
                USB_ROMCopy((const uint8_t*)((uint24_t)ROMptr),EP0_IN,EP0_SIZE);
                ROMptr += EP0_SIZE;
            }
            else{
                USB_RAMCopy((uint8_t*)((uint24_t)RAMptr),EP0_IN,EP0_SIZE);
                RAMptr += EP0_SIZE;
            }
            USB_Arm_EP0_IN(EP0_SIZE);
            bytes_2_send -= EP0_SIZE;
        }
        else{
            if(SendingFrom == ROM) USB_ROMCopy((const uint8_t*)((uint24_t)ROMptr),EP0_IN,bytes_2_send);
            else USB_RAMCopy((uint8_t*)((uint24_t)RAMptr),EP0_IN,bytes_2_send);
            USB_Arm_EP0_IN(bytes_2_send);
            bytes_2_send = 0;
        }
    }
    else{
        if(send_short){
            USB_Arm_EP0_IN(0);
            send_short = false;
        }
    }
#endif
}
/**
 * @fn void USB_OutControlTransfer(void)
 * 
 * @brief Processes a OUT Control (EP0 OUT) transaction. <b>So far not used</b>
 * 
 */
void USB_OutControlTransfer(void){
#if PINGPONG_MODE == PINGPONG_0_OUT || PINGPONG_MODE == PINGPONG_ALL_EP
    if(bytes_2_recv>EP0_SIZE){
        if(PINGPONG_PARITY == EVEN) USB_RAMCopy(EP0_OUT_EVEN,RAMptr,EP0_SIZE);
        else USB_RAMCopy(EP0_OUT_ODD,RAMptr,EP0_SIZE);
        RAMptr += EP0_SIZE;
        bytes_2_recv -= EP0_SIZE;
    }
    else{
        if(PINGPONG_PARITY == EVEN) USB_RAMCopy(EP0_OUT_EVEN,RAMptr,bytes_2_recv);
        else USB_RAMCopy(EP0_OUT_ODD,RAMptr,bytes_2_recv);
        RAMptr += EP0_SIZE;
        bytes_2_recv = 0;
    }
#else
    if(bytes_2_recv>EP0_SIZE){
        USB_RAMCopy(EP0_OUT,RAMptr,EP0_SIZE);
        RAMptr += EP0_SIZE;
        bytes_2_recv -= EP0_SIZE;
    }
    else{
        USB_RAMCopy(EP0_OUT,RAMptr,bytes_2_recv);
        RAMptr += bytes_2_recv;
        bytes_2_recv = 0;
    }
#endif
    if(bytes_2_recv == 0){
        
#ifdef USE_OUT_CONTROL_FINISHED
        if(USB_OutControlFinished()) USB_ArmINStatus();
        else{
            USB_RequestError();
        }
#else
        USB_ArmINStatus();
#endif
        ControlStage = STATUS_IN_STAGE;
    }
}
/**
 * @fn void USB_ROMCopy(const uint8_t *ROMaddress, uint8_t *EPaddress, uint8_t bytes)
 * 
 * @brief Copy values from ROM and place in endpoint.
 * 
 * @param[in] *ROMaddress Pointer to the address in ROM to copy from.
 * @param[in] *EPaddress Pointer to the endpoint address in RAM to copy to.
 * @param[in] bytes Amount of bytes to copy.
 * 
 */
void USB_ROMCopy(const uint8_t *ROMaddress, uint8_t *EPaddress, uint8_t bytes){
    uint8_t i;
    for(i=0;i<bytes;i++){
        EPaddress[i] = ROMaddress[i];
    }
}
/**
 * @fn void USB_RAMCopy(uint8_t *RAM1address, uint8_t *RAM2address, uint8_t bytes)
 * 
 * @brief Copy values from RAM and place in endpoint.
 * 
 * @param[in] *RAM1address Pointer to the address in RAM to copy from.
 * @param[in] *RAM2address Pointer to the endpoint address in RAM to copy to.
 * @param[in] bytes Amount of bytes to copy.
 * 
 */
void USB_RAMCopy(uint8_t *RAM1address, uint8_t *RAM2address, uint8_t bytes){
    uint8_t i;
    for(i=0;i<bytes;i++){
        RAM2address[i] = RAM1address[i];
    }
}
/**
 * @fn void USB_RAMSet(uint8_t val, uint8_t *RAMaddress, uint16_t bytes)
 * 
 * @brief Set an area of RAM to a particular value. Useful for clearing an endpoint.
 * 
 * @param[in] val Value to set to.
 * @param[in] *RAMaddress Pointer to the starting RAM address.
 * @param[in] bytes Amount of bytes to set.
 * 
 */
void USB_RAMSet(uint8_t val, uint8_t *RAMaddress, uint16_t bytes){
    uint16_t i;
    for(i=0;i<bytes;i++) RAMaddress[i] = val;
}
/**
 * @fn void USB_GET_STATUS(void)
 * 
 * @brief Handles GET_STATUS requests.
 * 
 */
static void USB_GET_STATUS(void){
    bool Perform_Request_Error = true;
    union{
        struct{
            union{
                unsigned SelfPowered    :1;
                unsigned Halt           :1;
            };
            unsigned RemoteWakeup       :1;
        };
        uint8_t Byte;
    }GetStatusReturnedData;
    
    GetStatusReturnedData.Byte = 0;
    
    if((USB_STATE == Address)||(USB_STATE == Configured)){
        switch(GetStatusData.bmRequestType_bits.Recipient){
            case Device:
                GetStatusReturnedData.SelfPowered = DEVS.Self_Powered;
                GetStatusReturnedData.RemoteWakeup = DEVS.Remote_Wakeup;
                Perform_Request_Error = false;
                break;
            case Interface:
                Perform_Request_Error = false;
                break;
            case Endpoint:
                if(USB_STATE == Address){
                    if(GetStatusData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber == EP0) Perform_Request_Error = false;
                }
                else{
                    if(GetStatusData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber<NUM_ENDPOINTS) Perform_Request_Error = false;
                }
                if(Perform_Request_Error == false){
                    GetStatusReturnedData.Halt = EPn_Stat.Array[GetStatusData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber][GetStatusData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction].Halt;
                }
                break;     
        }
    }
    if(Perform_Request_Error){USB_RequestError();}
    else{
#if PINGPONG_MODE == PINGPONG_ALL_EP
        if(EP0_IN_LAST_PPB == ODD){
            USB_RAMSet(0,EP0_IN_EVEN,8); // Clear EP ready for data
            EP0_IN_EVEN[0] = GetStatusReturnedData.Byte;
            USB_Arm_EP0_IN(BD0_IN_EVEN, 2);
        }
        else{
            USB_RAMSet(0,EP0_IN_ODD,8); // Clear EP ready for data
            EP0_IN_ODD[0] = GetStatusReturnedData.Byte;
            USB_Arm_EP0_IN(BD0_IN_ODD, 2);
        }
#else
        USB_RAMSet(0,EP0_IN,8); // Clear EP ready for data
        EP0_IN[0] = GetStatusReturnedData.Byte;
        USB_Arm_EP0_IN(2);

#endif
        ControlStage = DATA_IN_STAGE;
    }
}
/**
 * @fn void USB_SET_CLEAR_FEATURE(void)
 * 
 * @brief Handles SET_FEATURE and CLEAR_FEATURE requests.
 * 
 */
static void USB_SET_CLEAR_FEATURE(void){
    uint8_t BDT_Index;
    bool Perform_Request_Error = true;
    
    if((USB_STATE == Address)||(USB_STATE == Configured)){
        switch(SetClearFeatureData.bmRequestType_bits.Recipient){
            case Device:
                if(SetClearFeatureData.FeatureSelector == DEVICE_REMOTE_WAKEUP){
                    if(SetupData.bRequest == CLEAR_FEATURE) DEVS.Remote_Wakeup = REMOTE_WAKEUP_OFF;
                    else DEVS.Remote_Wakeup = REMOTE_WAKEUP_ON;
                    Perform_Request_Error = false;
                }
                break;
            case Endpoint:
                if(SetClearFeatureData.FeatureSelector == ENDPOINT_HALT){
                    if(USB_STATE == Configured){
                        if(SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber < NUM_ENDPOINTS) Perform_Request_Error = false;
                    }
                    else{
                        if(SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber == EP0) Perform_Request_Error = false;
                    }
                    if(Perform_Request_Error == false){
                        if(SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber == EP0){
#if PINGPONG_MODE == PINGPONG_ALL_EP                            
                            if(SetupData.bRequest == CLEAR_FEATURE){
                                EPn_Stat.Array[EP0][IN].Halt = 0;
                                BDT.Array[BD0_IN_EVEN].STAT = 0;
                                BDT.Array[BD0_IN_ODD].STAT = 0;
                            }
                            else{
                                EPn_Stat.Array[EP0][IN].Halt = 1;
                                USB_Stall_EP(&BDT.Array[BD0_IN_EVEN]);
                                USB_Stall_EP(&BDT.Array[BD0_IN_ODD]);
                            }
#else
                            if(SetupData.bRequest == CLEAR_FEATURE){
                                EPn_Stat.Array[EP0][IN].Halt = 0;
                                BDT.Array[BD0_IN].STAT = 0;
                            }
                            else{
                                EPn_Stat.Array[EP0][IN].Halt = 1;
                                USB_Stall_EP(&BDT.Array[BD0_IN]);
                            }
#endif
                        }
                        else{
#if PINGPONG_MODE == PINGPONG_DIS
                            BDT_Index = (SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber * 2u) + SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction;
#elif PINGPONG_MODE == PINGPONG_0_OUT
                            BDT_Index = (SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber * 2u) + 1u + SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction;
#elif PINGPONG_MODE == PINGPONG_1_15
                            BDT_Index = (SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber * 2u) + 1u + SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction;
#else
                            BDT_Index = (SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber * 2u) + 2u + (SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction * 2u);
#endif
                            if(SetupData.bRequest == CLEAR_FEATURE){
                                USB_AppClearHalt(BDT_Index, SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber, SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction);
                            }
                            else{
                                EPn_Stat.Array[SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.EndpointNumber][SetClearFeatureData.ZeroInterfaceEndpoint_bits.Endpoint_bits.Direction].Halt = 1;
                                USB_Stall_EP(&BDT.Array[BDT_Index]);
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                                BDT_Index++;
                                USB_Stall_EP(&BDT.Array[BDT_Index]);
#endif
                            }
                        }
                    }
                }
                break;
        }
    }
    if(Perform_Request_Error) {USB_RequestError();}
    else{
        USB_ArmINStatus();
        ControlStage = STATUS_IN_STAGE;
    }
}
/**
 * @fn void USB_SET_ADDRESS(void)
 * 
 * @brief Handles SET_ADDRESS requests.
 * 
 */
static void USB_SET_ADDRESS(void){
    saved_address = (uint8_t)SetAddressData.DeviceAddress;
    UpdateAddress = true;
    USB_ArmINStatus();
    ControlStage = STATUS_IN_STAGE;
}
/**
 * @fn void USB_GET_DESCRIPTOR(void)
 * 
 * @brief Handles GET_DESCRIPTOR requests.
 * 
 */
static void USB_GET_DESCRIPTOR(void){
    bool Perform_Request_Error = true;
    const uint16_t *ptr;
    
    switch(GetDescriptorData.DescriptorType){
        case DEVICE_DESC:
            ROMptr = (const uint8_t*)&Device_Descriptor;
            bytes_available = sizeof(Device_Descriptor);
            Perform_Request_Error = false;
            break;
        case DEVICE_QUALIFIER_DESC:
            break;
        case CONFIGURATION_DESC:
            if(GetDescriptorData.DescriptorIndex<NUM_CONFIGURATIONS){
                ROMptr = (const uint8_t*)CD_Ptr[GetDescriptorData.DescriptorIndex];
                ptr = (const uint16_t*)CD_Ptr[GetDescriptorData.DescriptorIndex];
                bytes_available = ptr[1];
                Perform_Request_Error = false;
            }
            break;
        case STRING_DESC:
            if(GetDescriptorData.DescriptorIndex<sizeof(SD_Ptr)){
                ROMptr = (const uint8_t*)SD_Ptr[GetDescriptorData.DescriptorIndex];
                bytes_available = ROMptr[0];
                Perform_Request_Error = false;
            }
            break;
        default:
            if(USB_GetClassDescriptor()) Perform_Request_Error = false;
            break;
    }
    if(Perform_Request_Error) {USB_RequestError();}
    else{
        if(bytes_available < GetDescriptorData.DescriptorLength){
            bytes_2_send = bytes_available;
            if(bytes_available % EP0_SIZE){
                send_short = false;
            }
            else send_short = true;
        }
        else{
            bytes_2_send = GetDescriptorData.DescriptorLength;
            send_short = false;
        }
        SendingFrom = ROM;
        
#if PINGPONG_MODE == PINGPONG_ALL_EP
        EP0_IN_LAST_PPB ^= 1;
        USB_InControlTransfer();
        if(bytes_2_send != 0){
            EP0_IN_DATA_TOGGLE_VAL ^= 1;
            EP0_IN_LAST_PPB ^= 1;
            USB_InControlTransfer();
        }
#else
        USB_InControlTransfer();
#endif
        ControlStage = DATA_IN_STAGE;
    }
}
/**
 * @fn void USB_SET_DESCRIPTOR(void)
 * 
 * @brief Handles SET_DESCRIPTOR requests.
 * 
 */
static void USB_SET_DESCRIPTOR(void){
    USB_RequestError();
}
/**
 * @fn void USB_GET_CONFIGURATION(void)
 * 
 * @brief Handles GET_CONFIGURATION requests.
 * 
 */
static void USB_GET_CONFIGURATION(void){
    uint8_t GetConfigurationReturnedData;
    if(USB_STATE == Configured) GetConfigurationReturnedData = CurrentConfiguration;
    else GetConfigurationReturnedData = 0; // If any state other than configured, return 0
#if PINGPONG_MODE == PINGPONG_ALL_EP
    if(EP0_IN_LAST_PPB == ODD){
        EP0_IN_EVEN[0] = GetConfigurationReturnedData;
        USB_Arm_EP0_IN(BD0_IN_EVEN, 1);
    }
    else{
        EP0_IN_ODD[0] = GetConfigurationReturnedData;
        USB_Arm_EP0_IN(BD0_IN_ODD, 1);
    }
#else
    EP0_IN[0] = GetConfigurationReturnedData;
    USB_Arm_EP0_IN(1);
#endif
    ControlStage = DATA_IN_STAGE;
}
/**
 * @fn void USB_SET_CONFIGURATION(void)
 * 
 * @brief Handles SET_CONFIGURATION requests.
 * 
 */
static void USB_SET_CONFIGURATION(void){
    if((USB_STATE == Address || USB_STATE == Configured) && (SetConfigurationData.ConfigurationValue < NUM_CONFIGURATIONS+1)){
        // Reset Ping-Pong Buffer Pointers to Even
#if (PINGPONG_MODE != PINGPONG_DIS)
        PPB_Reset = 1;
        NOP();
        NOP();
        NOP();
        NOP();
        PPB_Reset = 0;
        
        // Next_PPB starts on ODD
        for(uint8_t i=0;i<NUM_ENDPOINTS;i++){
            EPn_Stat.Array[i][0].Last_PPB = ODD;
            EPn_Stat.Array[i][1].Last_PPB = ODD;
        }
#endif
        USB_ArmINStatus();
        ControlStage = STATUS_IN_STAGE;
        
        CurrentConfiguration = SetConfigurationData.ConfigurationValue;      

        if(CurrentConfiguration){
            USB_InitApp();
            USB_STATE = Configured;
        }
        else USB_STATE = Address;
    }
    else {
        USB_RequestError();
    }
}
/**
 * @fn void USB_GET_INTERFACE(void)
 * 
 * @brief Handles GET_INTERFACE requests.
 * 
 */
static void USB_GET_INTERFACE(void){
#if NUM_ALT_INTERFACES == 0
    if(USB_STATE == Configured && GetInterfaceData.Interface<NUM_INTERFACES){
#if PINGPONG_MODE == PINGPONG_ALL_EP
        if(EP0_IN_LAST_PPB == ODD){
            EP0_IN_EVEN[0] = 0;
            USB_Arm_EP0_IN(BD0_IN_EVEN, 1);
        }
        else{
            EP0_IN_ODD[0] = 0;
            USB_Arm_EP0_IN(BD0_IN_ODD, 1);
        }
#else
        EP0_IN[0] = 0;
        USB_Arm_EP0_IN(1);
#endif
#else
    uint8_t GetInterfaceReturnedData;
    if(USB_STATE == Configured && USB_AppGetInterface(&GetInterfaceReturnedData,GetInterfaceData.Interface)){
#if PINGPONG_MODE == PINGPONG_ALL_EP
        if(EP0_IN_LAST_PPB == ODD){
            EP0_IN_EVEN[0] = GetInterfaceReturnedData;
            USB_Arm_EP0_IN(BD0_IN_EVEN, 1);
        }
        else{
            EP0_IN_ODD[0] = GetInterfaceReturnedData;
            USB_Arm_EP0_IN(BD0_IN_ODD, 1);
        }
#else
        EP0_IN[0] = GetInterfaceReturnedData;
        USB_Arm_EP0_IN(BD0_IN, 1);
#endif
#endif
        ControlStage = DATA_IN_STAGE;
    }
    else{USB_RequestError();}
}
/**
 * @fn void USB_SET_INTERFACE(void)
 * 
 * @brief Handles SET_INTERFACE requests.
 */
static void USB_SET_INTERFACE(void){
    if(USB_AppSetInterface(SetInterfaceData.AlternateSetting, SetInterfaceData.Interface)){
        USB_ArmINStatus();
    }
    else{
        USB_RequestError();
    }

}
/**
 * @fn void USB_SYNC_FRAME(void)
 * 
 * @brief Handles SYNC_FRAME requests.
 */
static void USB_SYNC_FRAME(void){
    USB_RequestError();
}