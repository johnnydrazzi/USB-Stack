/**
 * @file usb_msd.c
 * @brief Contains <i>Mass Storage Class</i> functions.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - MSD Library.
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
#include <stdint.h>
#include "USB.h"
#include "usb_msd.h"
#include "usb_ch9.h"
#include "usb_app.h"
#include "usb_scsi.h"

#define Dn 0 ///< Device expecting NO data transfer.
#define Di 1 ///< Device expecting IN data transfer.
#define Do 2 ///< Device expecting OUT data transfer.

static uint8_t MSD_ServiceCBW(void);
static void MSD_SetupCBW(void);
static void MSD_SetupCSW(void);
static bool MSD_ServiceRead10(void);
static bool MSD_ServiceWrite10(void);
static void MSD_CalcResidue(uint16_t CaseResult, uint32_t DeviceBytes);
static uint16_t MSD_Check13Cases(uint32_t DeviceBytes, uint8_t devExpect);
static bool MSD_CBW_Valid(void);
static void MSD_ResetSenseData(void);
static uint8_t MSD_FailCommand(uint8_t devExpect,uint8_t SK, uint8_t ASC, uint8_t ASCQ);
static uint8_t MSD_NoDataResponse(uint8_t status);
static uint8_t MSD_SendDataResponse(uint32_t DeviceBytes);
static void MSD_CauseBOMSR(void);

volatile static uint8_t MSD_STATE;
volatile static bool EndDataInShort;
volatile static bool WaitForBOMSR;
volatile static bool ClearHaltEvent;

volatile static uint8_t MSD_Tcnt;
volatile static uint8_t MSD_TputIndex;
volatile static uint8_t MSD_TgetIndex;

typedef struct{
    unsigned      :1;
    unsigned PPBI :1;
    unsigned DIR  :1;
    unsigned ENDP :4;
    unsigned      :1;
}USTAT_Type;

volatile static union{
    uint8_t Task[8];
    USTAT_Type Task_STAT[8];
}MSD_Tasks_Buffer;

#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
/**
 * @fn MSD_Arm_EP_OUT(uint8_t BDT_Index)
 * 
 * @brief Arms MSD EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm MSD EP OUT for a transaction.
 * 
 * @param[in] BDT_Index Buffer Descriptor Index.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * MSD_Arm_EP_OUT(MSD_BD_OUT_EVEN);
 * @endcode
 * </li></ul>
 */
void MSD_Arm_EP_OUT(uint8_t BDT_Index){
    if(EPn_Stat.Array[MSD_EP][OUT].Data_Toggle_Val) BDT.Array[BDT_Index].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[BDT_Index].STAT = _DTSEN; // DATA0
    BDT.Array[BDT_Index].CNT = MSD_EP_SIZE;
    BDT.Array[BDT_Index].STAT |= _UOWN;
}

/**
 * @fn void MSD_Arm_EP_IN(uint8_t BDT_Index, uint16_t cnt)
 * 
 * @brief Arms MSD EP IN Endpoint for a transaction.
 * 
 * The function is used to arm MSD EP IN for a transaction.
 * 
 * @param[in] BDT_Index Buffer Descriptor Index.
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * MSD_Arm_EP_IN(MSD_BD_IN_EVEN,MSD_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void MSD_Arm_EP_IN(uint8_t BDT_Index, uint16_t cnt){
    if(EPn_Stat.Array[MSD_EP][IN].Data_Toggle_Val) BDT.Array[BDT_Index].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[BDT_Index].STAT = _DTSEN; // DATA0
    BDT.Array[BDT_Index].CNT = cnt;
    BDT.Array[BDT_Index].STAT |= _UOWN;
}
#else

/**
 * @fn MSD_Arm_EP_OUT(void)
 * 
 * @brief Arms MSD EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm MSD EP OUT for a transaction.
 * 
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * MSD_Arm_EP_OUT();
 * @endcode
 * </li></ul>
 */
void MSD_Arm_EP_OUT(void){
    if(EPn_Stat.Array[MSD_EP][OUT].Data_Toggle_Val) BDT.Array[MSD_BD_OUT].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[MSD_BD_OUT].STAT = _DTSEN; // DATA0
    BDT.Array[MSD_BD_OUT].CNT = MSD_EP_SIZE;
    BDT.Array[MSD_BD_OUT].STAT |= _UOWN;
}

/**
 * @fn void MSD_Arm_EP_IN(uint16_t cnt)
 * 
 * @brief Arms MSD EP IN Endpoint for a transaction.
 * 
 * The function is used to arm MSD EP IN for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * MSD_Arm_EP_IN(MSD_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void MSD_Arm_EP_IN(uint16_t cnt){
    if(EPn_Stat.Array[MSD_EP][IN].Data_Toggle_Val) BDT.Array[MSD_BD_IN].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[MSD_BD_IN].STAT = _DTSEN; // DATA0
    BDT.Array[MSD_BD_IN].CNT = cnt;
    BDT.Array[MSD_BD_IN].STAT |= _UOWN;
}
#endif

/**
 * @fn bool MSD_ClassRequest(void)
 * 
 * @brief Used to service Class Requests for MSD.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool MSD_ClassRequest(void){
        if(SetupData.bRequest == GET_MAX_LUN) {USB_RequestError();} // Max Logic Unit Number
    else if(SetupData.bRequest == BOMSR){ // Bulk Only Mass Storage Reset
        if(SetupData.wValue != 0 || SetupData.wIndex != 0 || SetupData.wLength != 0) return false;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        if(MSD_EP_OUT_LAST_PPB == ODD && BDT.Array[MSD_BD_OUT_EVEN].STATbits.UOWN == 0) MSD_SetupCBW();
        else if (MSD_EP_OUT_LAST_PPB == EVEN && BDT.Array[MSD_BD_OUT_ODD].STATbits.UOWN == 0) MSD_SetupCBW();
#else
        if(!BDT.Array[MSD_BD_OUT].STATbits.UOWN) MSD_SetupCBW();
#endif
        MSD_Tcnt = 0;
        MSD_TputIndex = 0;
        MSD_TgetIndex = 0;
        
        WaitForBOMSR = false;
        USB_ArmINStatus();
        ControlStage = STATUS_IN_STAGE;
    }
    else{
        return false;
    }
    return true;
}

/**
 * @fn void MSD_Init(void)
 * 
 * @brief Used to initialize EPs used by the MSD library.
 * 
 * EP buffers are cleared, EP settings are configured, and the Buffer Descriptor Table values are setup.
 */
void MSD_Init(void){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    // BD settings
    BDT.Array[MSD_BD_OUT_EVEN].STAT = 0;
    BDT.Array[MSD_BD_OUT_EVEN].ADR = (uint16_t)MSD_EP_OUT_EVEN;
    BDT.Array[MSD_BD_OUT_ODD].STAT = 0;
    BDT.Array[MSD_BD_OUT_ODD].ADR = (uint16_t)MSD_EP_OUT_ODD;
    BDT.Array[MSD_BD_IN_EVEN].STAT = 0;
    BDT.Array[MSD_BD_IN_EVEN].ADR = (uint16_t)MSD_EP_IN_EVEN;
    BDT.Array[MSD_BD_IN_ODD].STAT = 0;
    BDT.Array[MSD_BD_IN_ODD].ADR = (uint16_t)MSD_EP_IN_ODD;
#else
    // BD settings
    BDT.Array[MSD_BD_OUT].STAT = 0;
    BDT.Array[MSD_BD_OUT].ADR = (uint16_t)MSD_EP_OUT;
    BDT.Array[MSD_BD_IN].STAT = 0;
    BDT.Array[MSD_BD_IN].ADR = (uint16_t)MSD_EP_IN;
#endif

    // EP Settings
    MSD_UEPbits.EPHSHK = 1;     // Handshaking enabled 
    MSD_UEPbits.EPCONDIS = 1;   // Don't allow SETUP
    MSD_UEPbits.EPOUTEN = 1;    // EP output enabled
    MSD_UEPbits.EPINEN = 1;     // EP input enabled
    
    EPn_Stat.Array[MSD_EP][OUT].Halt = 0;
    EPn_Stat.Array[MSD_EP][IN].Halt = 0;
    MSD_ClearEndpointToggle();
    
    WaitForBOMSR = false;
    EndDataInShort = false;
    ClearHaltEvent = false;
    
    MSD_Tcnt = 0;
    MSD_TputIndex = 0;
    MSD_TgetIndex = 0;
    
    MSD_ResetSenseData();
    
    MSD_SetupCBW();
}

/**
 * @fn void MSD_AddTask(void)
 * 
 * @brief Adds a MSD Task to the queue.
 * 
 * Adds a MSD Task to the queue so that MSD_Tasks() can service it.
 */
void MSD_AddTask(void){
    if(MSD_Tcnt < 4){
        MSD_Tasks_Buffer.Task[MSD_TputIndex++] = LastUSTAT.BYTE;
        if(MSD_TputIndex == 4) MSD_TputIndex = 0;
        MSD_Tcnt++;
    }
}

/**
 * @fn void MSD_Tasks(void)
 * 
 * @brief Services MSD tasks.
 * 
 * MSD_Tasks() must be run frequently in your main program loop. MSD_Tasks() 
 * doesn't run inside AppTasks() because MSD_Tasks() can delay the speed of
 * servicing the Control EP when the USTAT buffer has values for MSD_EPs. This
 * can prevent the arming of EP0_OUT in time for a SETUP_PACKET. The host sees
 * this as an error. Place MSD_AddTask() instead.
 */
void MSD_Tasks(void){
    USB_Interrupt_Enable = 0;
    if(MSD_Tcnt){
        if(MSD_TRANSACTION_DIR == OUT){
            MSD_EP_OUT_DATA_TOGGLE_VAL ^= 1;
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
            MSD_EP_OUT_LAST_PPB = MSD_PINGPONG_PARITY;
    #endif
            switch(MSD_STATE){
#ifdef USE_WRITE_10
                case MSD_WRITE_DATA:
                    if(MSD_ServiceWrite10()) MSD_SetupCSW();
                    break;
#endif
                case MSD_CBW:
                    MSD_STATE = MSD_ServiceCBW();
                    if(MSD_STATE == MSD_NO_DATA_STAGE) MSD_SetupCSW();
                    break;
            }
        }
        else{
            MSD_EP_IN_DATA_TOGGLE_VAL ^= 1;
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
            MSD_EP_IN_LAST_PPB = MSD_PINGPONG_PARITY;
    #endif
            switch(MSD_STATE){
                case MSD_READ_DATA:
                    if(MSD_ServiceRead10()){
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                        MSD_STATE = MSD_READ_FINISHED;
    #else
                        MSD_STATE = MSD_DATA_SENT;
    #endif
                    }
                    break;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                case MSD_READ_FINISHED:
                    MSD_STATE = MSD_DATA_SENT;
                    break;
#endif
                case MSD_DATA_SENT:
                    if(EndDataInShort){
                        EPn_Stat.Array[MSD_EP][IN].Halt = 1;
    #if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                        USB_Stall_EP(&BDT.Array[MSD_BD_IN_EVEN]);
                        USB_Stall_EP(&BDT.Array[MSD_BD_IN_ODD]);
    #else
                        USB_Stall_EP(&BDT.Array[MSD_BD_IN]);
    #endif
                        EndDataInShort = false;
                        break;
                    }
                    MSD_SetupCSW();
                    break;
                case MSD_CSW:
                    MSD_SetupCBW();
                    break;
            }
        }
        MSD_TgetIndex++;
        if(MSD_TgetIndex == 4) MSD_TgetIndex = 0;
        MSD_Tcnt--;
    }
    else if(ClearHaltEvent){
        if(MSD_STATE == MSD_WAIT_IVALID){
            MSD_SetupCBW();
        }
        else if(MSD_STATE == MSD_WAIT_ILLEGAL || MSD_STATE == MSD_DATA_SENT){
            MSD_SetupCSW();
        }
        ClearHaltEvent = false;
    }
    USB_Interrupt_Enable = 1;
}

/**
 * @fn void MSD_ClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir)
 * 
 * @brief Clear Endpoints used by MSD.
 * 
 * Used to inform the MSD library to clear a halt/stall condition on
 * it's endpoint.
 */
void MSD_ClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir){
    if(!WaitForBOMSR){
        EPn_Stat.Array[EP][dir].Data_Toggle_Val = 0;
        if(EPn_Stat.Array[EP][dir].Halt){
            EPn_Stat.Array[EP][dir].Halt = 0;
            BDT.Array[BDT_Index].STAT = 0;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
            BDT.Array[++BDT_Index].STAT = 0;
#endif
        }
        ClearHaltEvent = true;
    }
}

/**
 * @fn void MSD_ClearEndpointToggle(void)
 * 
 * @brief Used to clear MSD EP's Data Toggle values.
 * 
 */
void MSD_ClearEndpointToggle(void){
    MSD_EP_OUT_DATA_TOGGLE_VAL = 0;
    MSD_EP_IN_DATA_TOGGLE_VAL = 0;
}

/**
 * @fn uint8_t MSD_ServiceCBW(void)
 * 
 * @brief Used to service Command Block Wrapper on MSD's Endpoint.
 * 
 * The function parses the Command Block, and responds to SCSI Commands.
 * 
 * @return Returns the CBW Result.
 */
static uint8_t MSD_ServiceCBW(void){
    uint16_t CaseResult;
    uint8_t devExpect;

#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    uint8_t *IN_EPaddress;
    
    if(MSD_EP_IN_LAST_PPB == ODD){
        IN_EPaddress = MSD_EP_IN_EVEN;
    }
    else{
        IN_EPaddress = MSD_EP_IN_ODD;
    }
    
    if(MSD_EP_OUT_LAST_PPB == ODD) USB_RAMCopy(MSD_EP_OUT_ODD,CBW_Data.BYTES,31);
    else USB_RAMCopy(MSD_EP_OUT_EVEN,CBW_Data.BYTES,31);
#else
    USB_RAMCopy(MSD_EP_OUT,CBW_Data.BYTES,31);
#endif
    
    if(!MSD_CBW_Valid()){
        MSD_CauseBOMSR();
        return MSD_WAIT_IVALID;
    }
    CSW_Data.dCSWTag = CBW_Data.dCBWTag;
    
    switch(CBW_Data.CBWCB0[0]){
        case WRITE_10:
#if defined(USE_WRITE_10) && defined(USE_WR_PROTECT)
            if(MSD_WrProtect()){
#endif
#if !defined(USE_WRITE_10) || defined(USE_WR_PROTECT)
                return MSD_FailCommand(Do,DATA_PROTECT,ASC_WRITE_PROTECTED,ASCQ_WRITE_PROTECTED);
#endif
#if defined(USE_WRITE_10) && defined(USE_WR_PROTECT)
            }
#endif
        case READ_10:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) goto COMMAND_ERROR;
#endif
            R_W_10_Vars.TF_LEN_BYTES[0] = Read_10_Cmd.TF_LEN_BYTES[1];
            R_W_10_Vars.TF_LEN_BYTES[1] = Read_10_Cmd.TF_LEN_BYTES[0];
            
            if(R_W_10_Vars.TF_LEN == 0) return MSD_NoDataResponse(COMMAND_PASSED);
            
#ifdef USE_WRITE_10
            if(CBW_Data.CBWCB0[0] == READ_10) devExpect = Di;
            else devExpect = Do;
#else
            devExpect = Di;
#endif
            
            R_W_10_Vars.START_LBA_BYTES[0] = Read_10_Cmd.LBA_BYTES[3];
            R_W_10_Vars.START_LBA_BYTES[1] = Read_10_Cmd.LBA_BYTES[2];
            R_W_10_Vars.START_LBA_BYTES[2] = Read_10_Cmd.LBA_BYTES[1];
            R_W_10_Vars.START_LBA_BYTES[3] = Read_10_Cmd.LBA_BYTES[0];
            R_W_10_Vars.LBA = R_W_10_Vars.START_LBA;
            
            if((R_W_10_Vars.LBA + R_W_10_Vars.TF_LEN) > VOL_CAPACITY_IN_BLOCKS) return MSD_FailCommand(devExpect,ILLEGAL_REQUEST,ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE,ASCQ_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE);
            
            R_W_10_Vars.TF_LEN_IN_BYTES = ((uint32_t)R_W_10_Vars.TF_LEN)*BYTES_PER_BLOCK_LE;
            
            R_W_10_Vars.CBW_TF_LEN = CBW_Data.dCBWDataTransferLength;
            
            CaseResult = MSD_Check13Cases(R_W_10_Vars.TF_LEN_IN_BYTES,devExpect);
            MSD_CalcResidue(CaseResult, R_W_10_Vars.TF_LEN_IN_BYTES);
            
#ifdef USE_WRITE_10
            if(CaseResult & (CASE_11|CASE_12|CASE_13)){
                if(CaseResult == CASE_13) CSW_Data.bCSWStatus = PHASE_ERROR;
                else CSW_Data.bCSWStatus = COMMAND_PASSED;
                ByteOfSect = 0;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                MSD_Arm_EP_OUT((uint8_t)MSD_BD_OUT_EVEN + (MSD_EP_OUT_LAST_PPB ^ 1));
                MSD_EP_OUT_DATA_TOGGLE_VAL ^= 1;
                MSD_Arm_EP_OUT((uint8_t)MSD_BD_OUT_EVEN + MSD_EP_OUT_LAST_PPB);
#else
                MSD_Arm_EP_OUT();
#endif
                return MSD_WRITE_DATA;
            }
#endif
            if(CaseResult & (CASE_5|CASE_6|CASE_7)){
                if(CaseResult == CASE_7) CSW_Data.bCSWStatus = PHASE_ERROR;
                else CSW_Data.bCSWStatus = COMMAND_PASSED;
                ByteOfSect = 0;
#ifdef MSD_LIMITED_RAM
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP

                MSD_EP_IN_LAST_PPB ^= 1;
                MSD_ServiceRead10();
                
                MSD_EP_IN_DATA_TOGGLE_VAL ^= 1;
                MSD_EP_IN_LAST_PPB ^= 1;
                MSD_ServiceRead10();
#else
                MSD_ServiceRead10();
#endif
#else
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                MSD_EP_IN_LAST_PPB ^= 1;
                MSD_RxSector();
                MSD_ServiceRead10();
                
                MSD_EP_IN_DATA_TOGGLE_VAL ^= 1;
                MSD_EP_IN_LAST_PPB ^= 1;
                MSD_ServiceRead10();
#else
                MSD_RxSector();
                MSD_ServiceRead10();
#endif
#endif
                return MSD_READ_DATA;
            }
            if(CaseResult &  (CASE_2|CASE_3)){
                CSW_Data.bCSWStatus = PHASE_ERROR;
                return MSD_NO_DATA_STAGE;
            }
            CSW_Data.bCSWStatus = PHASE_ERROR;
            MSD_CauseBOMSR();
            return MSD_WAIT_ILLEGAL;
            
            
        case TEST_UNIT_READY:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) return MSD_NoDataResponse(COMMAND_FAILED);
#endif
            MSD_ResetSenseData();
#ifdef USE_TEST_UNIT_READY
            return MSD_NoDataResponse(MSD_TestUnitReady());
#else
            return MSD_NoDataResponse(COMMAND_PASSED);
#endif
            
#ifdef USE_PREVENT_ALLOW_MEDIUM_REMOVAL
        case PREVENT_ALLOW_MEDIUM_REMOVAL:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) return MSD_NoDataResponse(COMMAND_FAILED);
#endif
            goto COMMAND_ERROR;
        break;
#endif
            
            
        case REQUEST_SENSE:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()){
                MSD_ResetSenseData();
                Fixed_Format_Sense_Data.SENSE_KEY = NOT_READY;
                Fixed_Format_Sense_Data.ADDITIONAL_SENSE_CODE = ASC_MEDIUM_NOT_PRESENT;
                Fixed_Format_Sense_Data.ADDITIONAL_SENSE_CODE_QUALIFIER = ASCQ_MEDIUM_NOT_PRESENT;
            }
#endif
            BytesToTransfer.LB = Request_Sense_Cmd.ALLOCATION_LENGTH;
            BytesToTransfer.HB = 0;
            if(BytesToTransfer.val){
                if(BytesToTransfer.val > 18) BytesToTransfer.val = 18;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                USB_RAMCopy((uint8_t*)Fixed_Format_Sense_Data.BYTE,IN_EPaddress,BytesToTransfer.val);
#else
                USB_RAMCopy((uint8_t*)Fixed_Format_Sense_Data.BYTE,MSD_EP_IN,BytesToTransfer.val);
#endif
                return MSD_SendDataResponse(BytesToTransfer.val);
            }
            else return MSD_NoDataResponse(COMMAND_PASSED);
            
            
        case INQUIRY:
            BytesToTransfer.LB = Inquiry_Cmd.ALLOCATION_LENGTH_BYTES[1];
            BytesToTransfer.HB = Inquiry_Cmd.ALLOCATION_LENGTH_BYTES[0];
            if(BytesToTransfer.val){
                if(BytesToTransfer.val > 36) BytesToTransfer.val = 36;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                USB_ROMCopy((const uint8_t*)&SCSI_Inquiry_Data,IN_EPaddress,BytesToTransfer.val);
#else
                USB_ROMCopy((const uint8_t*)&SCSI_Inquiry_Data,MSD_EP_IN,BytesToTransfer.val);
#endif
                return MSD_SendDataResponse(BytesToTransfer.val);
            } 
            else return MSD_NoDataResponse(COMMAND_PASSED);
            
            
        case MODE_SENSE_6:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) goto COMMAND_ERROR;
#endif
            BytesToTransfer.LB = Mode_Sense_6_Cmd.ALLOCATION_LENGTH;
            BytesToTransfer.HB = 0;
            if(BytesToTransfer.val){
                if(BytesToTransfer.val > 4) BytesToTransfer.val = 4;
                Mode_Sense_Data.MODE_DATA_LENGTH=0x03;
                Mode_Sense_Data.MEDIUM_TYPE=0x00;
                Mode_Sense_Data.DEVICE_SPECIFIC_PARAMETER=0x00; // 0x00 for R/W, 0x80 for R-only
                Mode_Sense_Data.BLOCK_DESCRIPTOR_LENGTH=0x00;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                USB_RAMCopy((uint8_t*)&Mode_Sense_Data,IN_EPaddress,BytesToTransfer.val);
#endif
                return MSD_SendDataResponse(BytesToTransfer.val);
            }
            else return MSD_NoDataResponse(COMMAND_PASSED);
            
#ifdef USE_START_STOP_UNIT
        case START_STOP_UNIT:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) goto COMMAND_ERROR;
#endif
#ifdef USE_START_STOP_UNIT
            return MSD_NoDataResponse(MSD_StartStopUnit());
#else
            return MSD_NoDataResponse(COMMAND_PASSED);
#endif
#endif
            
            
        case READ_CAPACITY:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) goto COMMAND_ERROR;
#endif
            if((Read_Capacity_10_Cmd.LOGICAL_BLOCK_ADDRESS != 0)&&(Read_Capacity_10_Cmd.PMI == 0)){
                // CHECK CONDITION status, ILLEGAL REQUEST sense key, INVALID FIELD IN CDB sense code
                MSD_CauseBOMSR();
                return MSD_WAIT_ILLEGAL;
            }
            else{
                R_W_10_Vars.START_LBA_BYTES[0] = Read_Capacity_10_Cmd.LOGICAL_BLOCK_ADDRESS_BYTES[3];// Big-endian to little-endian
                R_W_10_Vars.START_LBA_BYTES[1] = Read_Capacity_10_Cmd.LOGICAL_BLOCK_ADDRESS_BYTES[2];
                R_W_10_Vars.START_LBA_BYTES[2] = Read_Capacity_10_Cmd.LOGICAL_BLOCK_ADDRESS_BYTES[1];
                R_W_10_Vars.START_LBA_BYTES[3] = Read_Capacity_10_Cmd.LOGICAL_BLOCK_ADDRESS_BYTES[0];
                
                R_W_10_Vars.LBA = R_W_10_Vars.START_LBA;
                
#ifdef USE_READ_CAPACITY
                MSD_ReadCapacity();
#else
                if(R_W_10_Vars.START_LBA>LAST_BLOCK_LE){
                    Read_Capacity_10_Data.RETURNED_LOGICAL_BLOCK_ADDRESS = 0xFFFFFFFFUL;
                }
                else{
                    Read_Capacity_10_Data.RETURNED_LOGICAL_BLOCK_ADDRESS = LAST_BLOCK_BE;
                }
                Read_Capacity_10_Data.BLOCK_LENGTH_IN_BYTES = BYTES_PER_BLOCK_BE;
#endif
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
                USB_RAMCopy((uint8_t*)&Read_Capacity_10_Data,IN_EPaddress,8);
#endif
                return MSD_SendDataResponse(8);
            }
            
#ifdef USE_VERIFY_10
        case VERIFY_10:
#ifdef USE_EXTERNAL_MEDIA
            if(!MSD_CheckForMedia()) goto COMMAND_ERROR;
#endif
            return MSD_NoDataResponse(COMMAND_PASSED);
#endif
        default:
#ifdef USE_EXTERNAL_MEDIA
            COMMAND_ERROR:
#endif
            if(CBW_Data.bCBWCBLength){
                if(CBW_Data.Direction) return MSD_FailCommand(Di,ILLEGAL_REQUEST,ASC_INVALID_COMMAND_OPCODE,ASCQ_INVALID_COMMAND_OPCODE);
                else return MSD_FailCommand(Do,ILLEGAL_REQUEST,ASC_INVALID_COMMAND_OPCODE,ASCQ_INVALID_COMMAND_OPCODE);
            }
            else return MSD_FailCommand(Dn,ILLEGAL_REQUEST,ASC_INVALID_COMMAND_OPCODE,ASCQ_INVALID_COMMAND_OPCODE);
    }
}

/**
 * @fn void MSD_SetupCBW(void)
 * 
 * @brief Used to arm MSD's OUT Endpoint ready for CBW.
 * 
 * The function arms MSD's OUT Endpoint ready for CBW.
 */
static void MSD_SetupCBW(void){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    MSD_Arm_EP_OUT((uint8_t)MSD_BD_OUT_EVEN + (MSD_EP_OUT_LAST_PPB ^ 1));
#else
    MSD_Arm_EP_OUT();
#endif
    MSD_STATE = MSD_CBW;
}

/**
 * @fn void MSD_SetupCSW(void)
 * 
 * @brief Used to service Command Status Wrapper on MSD's IN Endpoint.
 * 
 * The function loads the Status data and arms MSD's IN Endpoint for the CSW.
 */
static void MSD_SetupCSW(void){
    CSW_Data.dCSWSignature = CSW_SIG;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    if(MSD_EP_IN_LAST_PPB == ODD){
        USB_RAMCopy(CSW_Data.BYTES,MSD_EP_IN_EVEN,13);
        MSD_Arm_EP_IN(MSD_BD_IN_EVEN, 13);
    }
    else{
        USB_RAMCopy(CSW_Data.BYTES,MSD_EP_IN_ODD,13);
        MSD_Arm_EP_IN(MSD_BD_IN_ODD, 13);
    }
#else
    USB_RAMCopy(CSW_Data.BYTES,MSD_EP_IN,13);
    MSD_Arm_EP_IN(13);
#endif
    MSD_STATE = MSD_CSW;
}

/**
 * @fn void MSD_CalcResidue(uint16_t CaseResult, uint32_t DeviceBytes)
 * 
 * @brief Calculates residue for CSW.
 * 
 * @param CaseResult The result of checking for 13 error cases.
 * @param DeviceBytes The amount of bytes the device wishes to send.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * MSD_CalcResidue(CaseResult, 8);
 * @endcode
 * </li></ul>
 */
static void MSD_CalcResidue(uint16_t CaseResult, uint32_t DeviceBytes){
    if(CaseResult & (CASE_1|CASE_7|CASE_13)){
        CSW_Data.dCSWDataResidue = 0;
    }
    else if(CaseResult & (CASE_4|CASE_5|CASE_6|CASE_9|CASE_11|CASE_12)){
        CSW_Data.dCSWDataResidue = CBW_Data.dCBWDataTransferLength - DeviceBytes;
    }
    else{
        CSW_Data.dCSWDataResidue = CBW_Data.dCBWDataTransferLength;
    }
}

/**
 * @fn uint16_t MSD_Check13Cases(uint32_t DeviceBytes, uint8_t devExpect)
 * 
 * @brief Checks 13 possible error cases.
 * 
 * @param DeviceBytes The amount of bytes the device wishes to send.
 * @param devExpect The direction the device expects data to transfer.
 * @return CaseResult The case detected.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * CaseResult = MSD_Check13Cases(R_W_10_Vars.TF_LEN_IN_BYTES, Di);
 * @endcode
 * </li></ul>
 */
static uint16_t MSD_Check13Cases(uint32_t DeviceBytes, uint8_t devExpect){
    // Chapter 6.7 of MSC Spec 1.0
    /* ------ CASE ACTIONS ------ */
    // CASE_1: bCSWStatus = bCSWStatus = COMMAND_PASSED or COMMAND_FAILED, dCSWDataResidue = 0
    // CASE_2 & CASE_3: bCSWStatus = PHASE_ERROR, or STALL
    // CASE_4 & CASE_5: Send available data + fill data to make up full dCBWDataTransferLength,
    //                  bCSWStatus = COMMAND_PASSED or COMMAND_FAILED,
    //                  dCSWDataResidue = dCBWDataTransferLength - Relevant data sent
    // CASE_6: Send data, bCSWStatus = COMMAND_PASSED or COMMAND_FAILED, dCSWDataResidue = 0
    // CASE_7 & CASE_8: Send dCBWDataTransferLength zeros, bCSWStatus = PHASE_ERROR
    // CASE_9: & CASE_11: Receive bytes that can be interpreted, then stall.
    //                    bCSWStatus = COMMAND_PASSED or COMMAND_FAILED,
    //                    dCSWDataResidue = dCBWDataTransferLength - Relevant data received
    // CASE_12: Receive CBW_Data.dCBWDataTransferLength bytes,
    //          bCSWStatus = COMMAND_PASSED or COMMAND_FAILED,
    //          dCSWDataResidue = 0
    // CASE_10 & CASE_13: stall, bCSWStatus = PHASE_ERROR
    
    
    uint16_t CaseResult;
    
    switch(devExpect){
        case Dn:
            if(CBW_Data.dCBWDataTransferLength == 0) CaseResult = CASE_1;
            else if(CBW_Data.Direction == IN) CaseResult = CASE_4;
            else CaseResult = CASE_9;
            break;
        case Di:
            if(CBW_Data.dCBWDataTransferLength == 0) CaseResult = CASE_2;
            else if(CBW_Data.Direction == IN){
                if(CBW_Data.dCBWDataTransferLength == DeviceBytes) CaseResult = CASE_6;
                else if(CBW_Data.dCBWDataTransferLength > DeviceBytes) CaseResult = CASE_5;
                else CaseResult = CASE_7;
            }
            else CaseResult = CASE_10;
            break;
        case Do:
            if(CBW_Data.dCBWDataTransferLength == 0) CaseResult = CASE_3;
            else if(CBW_Data.Direction == OUT){
                if(CBW_Data.dCBWDataTransferLength == DeviceBytes) CaseResult = CASE_12;
                else if(CBW_Data.dCBWDataTransferLength > DeviceBytes) CaseResult = CASE_11;
                else CaseResult = CASE_13;
            }
            else CaseResult = CASE_8;
            break;
    }  
    return CaseResult;
}

/**
 * @fn bool MSD_CBW_Valid(void)
 * 
 * @brief Checks if CBW is valid.
 * 
 * @return Valid Returns true if valid.
 */
static bool MSD_CBW_Valid(void){
    bool valid_sts = true;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    if(MSD_EP_OUT_LAST_PPB == ODD){
        if(BDT.Array[MSD_BD_OUT_ODD].CNT != 31) valid_sts = false;
    }
    else{
        if(BDT.Array[MSD_BD_OUT_EVEN].CNT != 31) valid_sts = false;
    }
#else
    if(BDT.Array[MSD_BD_OUT].CNT != 31) valid_sts = false;
#endif
    if(CBW_Data.dCBWSignature != 0x43425355) valid_sts = false;
    if(!valid_sts) WaitForBOMSR = true;
    return valid_sts;
}

/**
 * @fn void MSD_CauseBOMSR(void)
 * 
 * @brief Causes a Bulk Only Mass Storage Reset.
 * 
 * The function stalls all MSD's Endpoints. The function is used when a serious error
 * has occurred and the devices wishes to re-sync with the computer.
 */
static void MSD_CauseBOMSR(void){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    EPn_Stat.Array[MSD_EP][IN].Halt = 1;
    USB_Stall_EP(&BDT.Array[MSD_BD_IN_EVEN]);
    USB_Stall_EP(&BDT.Array[MSD_BD_IN_ODD]);
    EPn_Stat.Array[MSD_EP][OUT].Halt = 1;
    USB_Stall_EP(&BDT.Array[MSD_BD_OUT_EVEN]);
    USB_Stall_EP(&BDT.Array[MSD_BD_OUT_ODD]);
#else
    EPn_Stat.Array[MSD_EP][IN].Halt = 1;
    USB_Stall_EP(&BDT.Array[MSD_BD_IN]);
    EPn_Stat.Array[MSD_EP][OUT].Halt = 1;
    USB_Stall_EP(&BDT.Array[MSD_BD_OUT]);
#endif
}

/**
 * @fn void MSD_ResetSenseData(void)
 * 
 * @brief Resets Sense Data to default (no error) values.
 */
static void MSD_ResetSenseData(void){
    uint8_t i;
    for(i=0;i<sizeof(Fixed_Format_Sense_Data.BYTE);i++) Fixed_Format_Sense_Data.BYTE[i] = 0;
    Fixed_Format_Sense_Data.RESPONSE_CODE = CURRENT_FIXED;
    Fixed_Format_Sense_Data.ADDITIONAL_SENSE_LENGTH = 0x0A;
//    Fixed_Format_Sense_Data.RESPONSE_CODE = CURRENT_FIXED;
//    Fixed_Format_Sense_Data.VALID = 0;
//    Fixed_Format_Sense_Data.Obsolete = 0;
//    Fixed_Format_Sense_Data.SENSE_KEY = NO_SENSE;
//    Fixed_Format_Sense_Data.RESERVED = 0;
//    Fixed_Format_Sense_Data.ILI = 0;
//    Fixed_Format_Sense_Data.EOM = 0;
//    Fixed_Format_Sense_Data.FILEMARK = 0;
//    Fixed_Format_Sense_Data.INFORMATION[0] = 0;
//    Fixed_Format_Sense_Data.INFORMATION[1] = 0;
//    Fixed_Format_Sense_Data.INFORMATION[2] = 0;
//    Fixed_Format_Sense_Data.INFORMATION[3] = 0;
//    Fixed_Format_Sense_Data.ADDITIONAL_SENSE_LENGTH = 0x0A;
//    Fixed_Format_Sense_Data.COMMAND_SPECIFIC_INFORMATION[0] = 0;
//    Fixed_Format_Sense_Data.COMMAND_SPECIFIC_INFORMATION[1] = 0;
//    Fixed_Format_Sense_Data.COMMAND_SPECIFIC_INFORMATION[2] = 0;
//    Fixed_Format_Sense_Data.COMMAND_SPECIFIC_INFORMATION[3] = 0;
//    Fixed_Format_Sense_Data.ADDITIONAL_SENSE_CODE = 0;
//    Fixed_Format_Sense_Data.ADDITIONAL_SENSE_CODE_QUALIFIER = 0;
//    Fixed_Format_Sense_Data.FIELD_REPLACEABLE_UNIT_CODE = 0;
//    Fixed_Format_Sense_Data.SENSE_KEY_SPECIFIC[0] = 0;
//    Fixed_Format_Sense_Data.SENSE_KEY_SPECIFIC[1] = 0;
//    Fixed_Format_Sense_Data.SENSE_KEY_SPECIFIC[2] = 0;
}
/**
 * @fn uint8_t MSD_IllegalCommand(uint8_t devExpect, uint8_t ASC, uint8_t ASCQ)
 * 
 * @brief Response to an illegal/unsupported SCSI command.
 * 
 * The function will stall either MSD's IN Endpoint or OUT depending on data direction and set the
 * sense data to values indicating an illegal SCSI request occurred. A COMMAND_FAILED status
 * is returned in the CSW stage.
 * 
 * @param devExpect The direction the device expects data to transfer.
 * @param ASC Additional Sense Code to be updated.
 * @param ASCQ Additional Sense Code Qualifier to be updated.
 * @return RESULT Returns the next mass storage state.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * return MSD_IllegalCommand(Di, ASC_INVALID_COMMAND_OPCODE, ASCQ_INVALID_COMMAND_OPCODE);
 * @endcode
 * </li></ul>
 */
static uint8_t MSD_FailCommand(uint8_t devExpect,uint8_t SK, uint8_t ASC, uint8_t ASCQ){
    uint8_t result;
    if(CBW_Data.dCBWDataTransferLength != 0){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        if(devExpect == Di){
            EPn_Stat.Array[MSD_EP][IN].Halt = 1;
            USB_Stall_EP(&BDT.Array[MSD_BD_IN_EVEN]);
            USB_Stall_EP(&BDT.Array[MSD_BD_IN_ODD]);
        }
        else{
            EPn_Stat.Array[MSD_EP][OUT].Halt = 1;
            USB_Stall_EP(&BDT.Array[MSD_BD_OUT_EVEN]);
            USB_Stall_EP(&BDT.Array[MSD_BD_OUT_ODD]);
        }
#else
        if(devExpect == Di){
            EPn_Stat.Array[MSD_EP][IN].Halt = 1;
            USB_Stall_EP(&BDT.Array[MSD_BD_IN]);
        }
        else{
            EPn_Stat.Array[MSD_EP][OUT].Halt = 1;
            USB_Stall_EP(&BDT.Array[MSD_BD_OUT]);
        }
#endif
        result = MSD_WAIT_ILLEGAL;
    }
    else result = MSD_NO_DATA_STAGE;
    CSW_Data.dCSWDataResidue = CBW_Data.dCBWDataTransferLength;
    CSW_Data.bCSWStatus = COMMAND_FAILED;
    MSD_ResetSenseData();
    Fixed_Format_Sense_Data.SENSE_KEY = SK;
    Fixed_Format_Sense_Data.ADDITIONAL_SENSE_CODE = ASC;
    Fixed_Format_Sense_Data.ADDITIONAL_SENSE_CODE_QUALIFIER = ASCQ;
    return result;
}

/**
 * @fn uint8_t MSD_NoDataResponse(uint8_t status)
 * 
 * @brief Handles SCSI commands where no data stage is expected.
 * 
 * @param status The COMMAND_STATUS value returned in the CSW stage.
 * @return RESULT Returns the next mass storage state.
 */
static uint8_t MSD_NoDataResponse(uint8_t status){
    uint16_t CaseResult = MSD_Check13Cases(0,Dn);
    if(CaseResult == CASE_1){
        CSW_Data.dCSWDataResidue = 0;
        CSW_Data.bCSWStatus = status;
        return MSD_NO_DATA_STAGE;
    }
    MSD_CalcResidue(CaseResult, 0);
    MSD_CauseBOMSR();
    return MSD_WAIT_ILLEGAL;
}

/**
 * @fn uint8_t MSD_SendDataResponse(uint32_t DeviceBytes)
 * 
 * @brief Handles SCSI commands where data stage is expected.
 * 
* @param DeviceBytes The amount of bytes the device wishes to send.
 * @return RESULT Returns the next mass storage state.
 */
static uint8_t MSD_SendDataResponse(uint32_t DeviceBytes){
    uint16_t CaseResult = MSD_Check13Cases(DeviceBytes,Di);
    uint8_t return_val;
    
    MSD_CalcResidue(CaseResult,DeviceBytes);
    if(CaseResult & (CASE_2 | CASE_5 | CASE_6 | CASE_7)){
        if(CaseResult & (CASE_5 | CASE_6 | CASE_7)){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
            MSD_Arm_EP_IN((uint8_t)MSD_BD_IN_EVEN + (MSD_EP_IN_LAST_PPB ^ 1), DeviceBytes);
#else
            MSD_Arm_EP_IN(DeviceBytes);
#endif
            return_val = MSD_DATA_SENT;
        }
        else return_val = MSD_NO_DATA_STAGE;
        
        if(CaseResult & (CASE_5 | CASE_6)) CSW_Data.bCSWStatus = COMMAND_PASSED;
        else CSW_Data.bCSWStatus = PHASE_ERROR;

        if(CaseResult == CASE_5) EndDataInShort = true;
        
        return return_val;
    }
    else{
        MSD_CauseBOMSR();
        return MSD_WAIT_ILLEGAL;
    }
    
}

/**
 * @fn bool MSD_ServiceRead10(void)
 * 
 * @brief Services the READ_10 SCSI Command.
 * 
 * @return Returns true when all data is sent.
 */
static bool MSD_ServiceRead10(void){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
#ifndef MSD_LIMITED_RAM
    uint8_t *EPaddress;
#endif
    uint8_t BDT_Index;
    
    if(MSD_EP_IN_LAST_PPB == ODD){
#ifndef MSD_LIMITED_RAM
        EPaddress = MSD_EP_IN_ODD;
#endif
        BDT_Index = MSD_BD_IN_ODD;
    }
    else{
#ifndef MSD_LIMITED_RAM
        EPaddress = MSD_EP_IN_EVEN;
#endif
        BDT_Index = MSD_BD_IN_EVEN;
    }
    
#ifdef MSD_LIMITED_RAM
    MSD_RxSector();
#else
    for(uint16_t i=0;i<MSD_EP_SIZE;i++) EPaddress[i] = MSD_SectData[i + ByteOfSect]; // Load EP size worth of data from the MSD_SectData buffer.
#endif
    
    ByteOfSect += MSD_EP_SIZE;
    if(ByteOfSect == BYTES_PER_BLOCK_LE){ // More than one sector is required. Last bytes of sector were sent, increment the address, and load new sector.
        R_W_10_Vars.LBA++;
#ifndef MSD_LIMITED_RAM
        MSD_RxSector();
#endif
        ByteOfSect = 0;
    }
    
    MSD_Arm_EP_IN(BDT_Index, MSD_EP_SIZE);
    
    R_W_10_Vars.CBW_TF_LEN -= MSD_EP_SIZE;
    if(R_W_10_Vars.CBW_TF_LEN == 0){
        MSD_EP_IN_DATA_TOGGLE_VAL ^= 1;
        return true;
    }
    else return false;
    
#else
#ifdef MSD_LIMITED_RAM
    MSD_RxSector();
#else
    for(uint16_t i=0;i<MSD_EP_SIZE;i++) MSD_EP_IN[i] = MSD_SectData[i + ByteOfSect]; // Load EP size worth of data from the MSD_SectData buffer.
#endif

    ByteOfSect += MSD_EP_SIZE;
    if(ByteOfSect == BYTES_PER_BLOCK_LE){ // More than one sector is required. Last bytes of sector were sent, increment the address, and load new sector.
        R_W_10_Vars.LBA++;
#ifndef MSD_LIMITED_RAM
        MSD_RxSector();
#endif
        ByteOfSect = 0;
    }

    MSD_Arm_EP_IN(MSD_EP_SIZE);

    R_W_10_Vars.CBW_TF_LEN -= MSD_EP_SIZE;
    if(R_W_10_Vars.CBW_TF_LEN == 0) return true;
    else return false;
#endif
}

/**
 * @fn bool MSD_ServiceWrite10(void)
 * 
 * @brief Services the WRITE_10 SCSI Command.
 * 
 * @return Returns true when all data is received.
 */
static bool MSD_ServiceWrite10(void){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
#ifndef MSD_LIMITED_RAM
    uint8_t *EPaddress;
    
    if(MSD_EP_OUT_LAST_PPB == ODD) EPaddress = MSD_EP_OUT_ODD;
    else EPaddress = MSD_EP_OUT_EVEN;
#endif

#ifdef MSD_LIMITED_RAM
    MSD_TxSector();
#else
    for(uint16_t i=0;i<MSD_EP_SIZE;i++) MSD_SectData[i + ByteOfSect] = EPaddress[i] ; // Load EP size worth of data from EP to MSD_SectData buffer.
#endif
    ByteOfSect += MSD_EP_SIZE;
    if(ByteOfSect == BYTES_PER_BLOCK_LE){
#ifndef MSD_LIMITED_RAM
        MSD_TxSector();
#endif
        R_W_10_Vars.LBA++;
        ByteOfSect = 0;
    }

    R_W_10_Vars.CBW_TF_LEN -= MSD_EP_SIZE;
    
    if(R_W_10_Vars.CBW_TF_LEN){
        MSD_Arm_EP_OUT((uint8_t)MSD_BD_OUT_EVEN + MSD_EP_OUT_LAST_PPB);
        return false;
    }
    else{
        MSD_EP_OUT_DATA_TOGGLE_VAL ^= 1;
        return true;
    }
    
#else
#ifdef MSD_LIMITED_RAM
    MSD_TxSector();
#else
    for(uint16_t i=0;i<MSD_EP_SIZE;i++) MSD_SectData[i + ByteOfSect] = MSD_EP_OUT[i] ; // Load EP size worth of data from EP to MSD_SectData buffer.
#endif
    ByteOfSect += MSD_EP_SIZE;
    if(ByteOfSect == BYTES_PER_BLOCK_LE){
#ifndef MSD_LIMITED_RAM
        MSD_TxSector();
#endif
        R_W_10_Vars.LBA++;
        ByteOfSect = 0;
    }
    R_W_10_Vars.CBW_TF_LEN -= MSD_EP_SIZE;
    if(R_W_10_Vars.CBW_TF_LEN == 0) return true;
    else{
        MSD_Arm_EP_OUT();
        return false;
    }
#endif
}