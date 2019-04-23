/**
 * @file usb_hid.c
 * @brief Contains <i>Human Interface Device Class</i> functions.
 * @author John Izzard
 * @date 13/02/2019
 * 
 * USB uC - HID Library.
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
#include <stdint.h>
#include <stdbool.h>
#include "USB.h"
#include "usb_hid.h"
#include "usb_ch9.h"
#include "usb_descriptors.h"

struct{
    unsigned :8;
    unsigned :8;
    uint8_t Report_ID;
    uint8_t Report_Type;
    uint16_t Interface;
    uint16_t Report_Length;
}GetSetReportData @SETUP_DATA_ADDR;

struct{
    unsigned :8;
    unsigned :8;
    uint8_t Report_ID;
    unsigned :8;
    uint16_t Interface;
    uint16_t wLength;
}GetIdleData @SETUP_DATA_ADDR;

struct{
    unsigned :8;
    unsigned :8;
    uint8_t Report_ID;
    uint8_t Duration;
    uint16_t Interface;
}SetIdleData @SETUP_DATA_ADDR;

bool HID_GET_REPORT(void);
bool HID_SET_REPORT(void);
bool HID_GET_IDLE(void);
bool HID_SET_IDLE(void);

// HID Requests
enum{
    GET_REPORT = 1,
    GET_IDLE,
    GET_PROTOCOL,
    SET_REPORT = 9,
    SET_IDLE,
    SET_PROTOCOL
};

#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
/**
 * @fn HID_Arm_EP_OUT(uint8_t BDT_Index)
 * 
 * @brief Arms HID EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm HID EP OUT for a transaction.
 * 
 * @param[in] BDT_Index Buffer Descriptor Index.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * HID_Arm_EP_OUT(HID_BD_OUT_EVEN);
 * @endcode
 * </li></ul>
 */
void HID_Arm_EP_OUT(uint8_t BDT_Index){
    if(EPn_Stat.Array[HID_EP][OUT].Data_Toggle_Val) BDT.Array[BDT_Index].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[BDT_Index].STAT = _DTSEN; // DATA0
    BDT.Array[BDT_Index].CNT = HID_EP_SIZE;
    BDT.Array[BDT_Index].STAT |= _UOWN;
}

/**
 * @fn void HID_Arm_EP_IN(uint8_t BDT_Index, uint16_t cnt)
 * 
 * @brief Arms HID EP IN Endpoint for a transaction.
 * 
 * The function is used to arm HID EP IN for a transaction.
 * 
 * @param[in] BDT_Index Buffer Descriptor Index.
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * HID_Arm_EP_IN(HID_BD_IN_EVEN,HID_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void HID_Arm_EP_IN(uint8_t BDT_Index, uint16_t cnt){
    if(EPn_Stat.Array[HID_EP][IN].Data_Toggle_Val) BDT.Array[BDT_Index].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[BDT_Index].STAT = _DTSEN; // DATA0
    BDT.Array[BDT_Index].CNT = cnt;
    BDT.Array[BDT_Index].STAT |= _UOWN;
}
#else

/**
 * @fn HID_Arm_EP_OUT(void)
 * 
 * @brief Arms HID EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm HID EP OUT for a transaction.
 * 
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * HID_Arm_EP_OUT();
 * @endcode
 * </li></ul>
 */
void HID_Arm_EP_OUT(void){
    if(EPn_Stat.Array[HID_EP][OUT].Data_Toggle_Val) BDT.Array[HID_BD_OUT].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[HID_BD_OUT].STAT = _DTSEN; // DATA0
    BDT.Array[HID_BD_OUT].CNT = HID_EP_SIZE;
    BDT.Array[HID_BD_OUT].STAT |= _UOWN;
}

/**
 * @fn void HID_Arm_EP_IN(uint16_t cnt)
 * 
 * @brief Arms HID EP IN Endpoint for a transaction.
 * 
 * The function is used to arm HID EP IN for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * HID_Arm_EP_IN(HID_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void HID_Arm_EP_IN(uint16_t cnt){
    if(EPn_Stat.Array[HID_EP][IN].Data_Toggle_Val) BDT.Array[HID_BD_IN].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[HID_BD_IN].STAT = _DTSEN; // DATA0
    BDT.Array[HID_BD_IN].CNT = cnt;
    BDT.Array[HID_BD_IN].STAT |= _UOWN;
}
#endif

/**
 * @fn bool HID_ClassRequest(void)
 * 
 * @brief Used to service Class Requests for HID.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool HID_ClassRequest(void){
    switch(SetupData.bRequest){
        case GET_REPORT:
            return HID_GET_REPORT();
#ifdef USE_SET_REPORT
        case SET_REPORT:
#endif /* SET_REPORT */
            return HID_SET_REPORT();
#ifdef USE_GET_IDLE
        case GET_IDLE:
            return HID_GET_IDLE();
#endif /* USE_GET_IDLE */
#ifdef USE_SET_IDLE
        case SET_IDLE:
            return HID_SET_IDLE();
#endif /* USE_SET_IDLE */
#ifdef USE_GET_PROTOCOL
        case GET_PROTOCOL:
#endif /* USE_GET_PROTOCOL */
#ifdef USE_SET_PROTOCOL
        case SET_PROTOCOL:
#endif /* USE_SET_PROTOCOL */
        default:
            return false;
    }
}

/**
 * @fn bool HID_GetClassDescriptor(void)
 * 
 * @brief Used to service Get Class Descriptor Requests for HID.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool HID_GetClassDescriptor(void){
    switch(GetDescriptorData.DescriptorType){
        case HID_DESC:
            ROMptr = (const uint8_t*)&Config_Descriptor0.HID_Descriptor;
            bytes_available = 9;
            return true;
        case HID_REPORT_DESC:
            ROMptr = HID_Report_Descriptor;
            bytes_available = sizeof(HID_Report_Descriptor);
            return true;
        default:
            return false;
    }
}

/**
 * @fn void HID_Init(void)
 * 
 * @brief Used to initialize EPs used by the HID library.
 * 
 * EP buffers are cleared, EP settings are configured, and the Buffer Descriptor Table values are setup.
 */
void HID_Init(void){
    uint8_t i;

#if HID_NUM_OUT_REPORTS != 0
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    for(i=0;i<8;i++){HID_EP_OUT_EVEN[i] = 0;}
    for(i=0;i<8;i++){HID_EP_OUT_ODD[i] = 0;}
    
    BDT.Array[HID_BD_OUT_EVEN].STAT = 0;
    BDT.Array[HID_BD_OUT_EVEN].ADR = HID_EP_OUT_EVEN_BUFFER_BASE_ADDR;
    BDT.Array[HID_BD_OUT_ODD].STAT = 0;
    BDT.Array[HID_BD_OUT_ODD].ADR = HID_EP_OUT_ODD_BUFFER_BASE_ADDR;
#else
    for(i=0;i<8;i++){HID_EP_OUT[i] = 0;}
    
    BDT.Array[HID_BD_OUT].STAT = 0;
    BDT.Array[HID_BD_OUT].ADR = HID_EP_OUT_BUFFER_BASE_ADDR;
#endif
#endif
#if HID_NUM_IN_REPORTS != 0
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    for(i=0;i<8;i++){HID_EP_IN_EVEN[i] = 0;}
    for(i=0;i<8;i++){HID_EP_IN_ODD[i] = 0;}
    
    BDT.Array[HID_BD_IN_EVEN].STAT = 0;
    BDT.Array[HID_BD_IN_EVEN].ADR = HID_EP_IN_EVEN_BUFFER_BASE_ADDR;
    BDT.Array[HID_BD_IN_ODD].STAT = 0;
    BDT.Array[HID_BD_IN_ODD].ADR = HID_EP_IN_ODD_BUFFER_BASE_ADDR;
#else
    for(i=0;i<8;i++){HID_EP_IN[i] = 0;}
    
    BDT.Array[HID_BD_IN].STAT = 0;
    BDT.Array[HID_BD_IN].ADR = HID_EP_IN_BUFFER_BASE_ADDR;
#endif
#endif
    
    // EP Settings
    HID_UEPbits.EPHSHK = 1;     // Handshaking enabled 
    HID_UEPbits.EPCONDIS = 0;  // Don't allow SETUP
#if HID_NUM_OUT_REPORTS != 0
    HID_UEPbits.EPOUTEN = 1;    // EP output enabled
    EPn_Stat.Array[HID_EP][OUT].Halt = 0;
#endif
#if HID_NUM_IN_REPORTS != 0
    HID_UEPbits.EPINEN = 1;     // EP input enabled
    EPn_Stat.Array[HID_EP][IN].Halt = 0;
#if HID_NUM_IN_REPORTS > 1
    for(i=0;i<HID_NUM_IN_REPORTS;i++){
		SentReportFlag[i] = true;
        In_Report_Settings[i].Idle_Count_Overflow = false;
        In_Report_Settings[i].Idle_Count = 0;
        In_Report_Settings[i].Idle_Duration_1ms = DEFAULT_IDLE;
        In_Report_Settings[i].Idle_Duration_4ms = DEFAULT_IDLE/4;
    }
#else
    SentReportFlag[0] = true;
	In_Report_Settings[0].Idle_Count_Overflow = false;
    In_Report_Settings[0].Idle_Count = 0;
    In_Report_Settings[0].Idle_Duration_1ms = DEFAULT_IDLE;
    In_Report_Settings[0].Idle_Duration_4ms = DEFAULT_IDLE/4;
#endif
#endif
    HID_ClearEndpointToggle();

#if HID_NUM_OUT_REPORTS != 0
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
    HID_Arm_EP_OUT(HID_BD_OUT_EVEN);
#else
    HID_Arm_EP_OUT();
#endif
#endif
    ReportSent = true;
}

/**
 * @fn void HID_Tasks(void)
 * 
 * @brief Services HID tasks.
 */
void HID_Tasks(void){
    if(TRANSACTION_DIR == IN){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        HID_EP_IN_LAST_PPB = PINGPONG_PARITY;
#endif
        HID_EP_IN_DATA_TOGGLE_VAL ^= 1;
        HID_SetSentReportFlag();
    }
    else{
#if HID_NUM_OUT_REPORTS != 0
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        HID_EP_OUT_LAST_PPB = PINGPONG_PARITY;
#endif
        HID_EP_OUT_DATA_TOGGLE_VAL ^= 1;
        
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        if(HID_EP_OUT_LAST_PPB == ODD){
            USB_RAMCopy((uint8_t*)HID_EP_OUT_ODD_BUFFER_BASE_ADDR,(uint8_t*)&Out_Report1,sizeof(Out_Report1));
            HID_Out();
        }
        else{
            USB_RAMCopy((uint8_t*)HID_EP_OUT_EVEN_BUFFER_BASE_ADDR,(uint8_t*)&Out_Report1,sizeof(Out_Report1));
            HID_Out();
        }
#else
        USB_RAMCopy((uint8_t*)HID_EP_OUT_BUFFER_BASE_ADDR,(uint8_t*)&Out_Report1,sizeof(Out_Report1));
        HID_Out();
#endif
#endif
    }
}

/**
 * @fn void HID_ClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir)
 * 
 * @brief Clear Endpoints used by HID.
 * 
 * Used to inform the HID library to clear a halt/stall condition on
 * it's endpoint.
 */
void HID_ClearHalt(uint8_t BDT_Index, uint8_t EP, uint8_t dir){
    EPn_Stat.Array[EP][dir].Data_Toggle_Val = 0;
    if(EPn_Stat.Array[EP][dir].Halt){
        EPn_Stat.Array[EP][dir].Halt = 0;
        BDT.Array[BDT_Index].STAT = 0;
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        BDT.Array[++BDT_Index].STAT = 0;
#endif
    }
    if(dir == IN) HID_SetSentReportFlag();
}

#define REPORT_INPUT   1
#define REPORT_OUTPUT  2
#define REPORT_FEATURE 3

/**
 * @fn bool HID_GET_REPORT(void)
 * 
 * @brief Returns Report on EP0.
 * 
 * The Get_Report request allows the host to receive a report via the Control pipe
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool HID_GET_REPORT(void){
    if(GetSetReportData.Report_Type == REPORT_INPUT){
#if HID_NUM_IN_REPORTS == 0
        return false;
#else
#if HID_NUM_REPORT_IDS == 0
        if(GetSetReportData.Report_ID != 0) return false;
        RAMptr = (uint8_t*)In_Report_Ptr[0];
        bytes_available = In_Report_Size[0];
#else
        if(GetSetReportData.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(GetSetReportData.Report_ID == 0) return false;
        RAMptr = (uint8_t*)In_Report_Ptr[GetSetReportData.Report_ID-1u];
        bytes_available = In_Report_Size[GetSetReportData.Report_ID-1u];
#endif
#endif
    }
    else if(GetSetReportData.Report_Type == REPORT_FEATURE){
#if HID_NUM_FEATURE_REPORTS == 0
        return false;
#else
#if HID_NUM_REPORT_IDS == 0
        if(GetSetReportData.Report_ID != 0) return false;
        RAMptr = (uint8_t*)Feature_Report_Ptr[0];
        bytes_available = Feature_Report_Size[0];
#else
        if(GetSetReportData.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(GetSetReportData.Report_ID == 0) return false;
        RAMptr = (uint8_t*)Feature_Report_Ptr[GetSetReportData.Report_ID-1];
        bytes_available = Feature_Report_Size[GetSetReportData.Report_ID-1];
#endif
#endif
    }
    else return false;
#if HID_NUM_IN_REPORTS != 0 || HID_NUM_FEATURE_REPORTS != 0
    if(GetSetReportData.Report_Length > bytes_available){
        bytes_2_send = bytes_available;
        if(GetSetReportData.Report_Length % bytes_available) send_short = true;
        else send_short = false;
    }
    else{
        bytes_2_send = GetSetReportData.Report_Length;
        send_short = false;
    }
    SendingFrom = RAM;
    USB_InControlTransfer();
    ControlStage = DATA_IN_STAGE;
    return true;
#endif
}


#ifdef USE_SET_REPORT
/**
 * @fn bool HID_SET_REPORT(void)
 * 
 * @brief Modifies Report on EP0.
 * 
 * The Set_Report request allows the host to send a report to the device, 
 * possibly setting the state of input, output, or feature controls.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool HID_SET_REPORT(void){
    if(GetSetReportData.Report_Type == REPORT_OUTPUT){
#if HID_NUM_OUT_REPORTS == 0
        return false;
#else
#if HID_NUM_REPORT_IDS == 0
        if(GetSetReportData.Report_ID != 0) return false;
        RAMptr = (uint8_t*)Out_Report_Ptr[0];
        bytes_available = Out_Report_Size[0];
#else
        if(GetSetReportData.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(GetSetReportData.Report_ID == 0) return false;
        RAMptr = (uint8_t*)Out_Report_Ptr[GetSetReportData.Report_ID-1u];
        bytes_available = Out_Report_Size[GetSetReportData.Report_ID-1u];
#endif
#endif
    }
    else if(GetSetReportData.Report_Type == REPORT_FEATURE){
#if HID_NUM_FEATURE_REPORTS == 0
        return false;
#else
#if HID_NUM_REPORT_IDS == 0
        if(GetSetReportData.Report_ID != 0) return false;
        RAMptr = (uint8_t*)Feature_Report_Ptr[0];
        bytes_available = Feature_Report_Size[0];
#else
        if(GetSetReportData.Report_ID > HID_NUM_REPORT_IDS) return false;
        if(GetSetReportData.Report_ID == 0) return false;
        RAMptr = (uint8_t*)Feature_Report_Ptr[GetSetReportData.Report_ID-1];
        bytes_available = Feature_Report_Size[GetSetReportData.Report_ID-1];
#endif
#endif
    }
    else return false;
#if HID_NUM_OUT_REPORTS != 0 || HID_NUM_FEATURE_REPORTS != 0
    if(GetSetReportData.Report_Length > bytes_available) return false;
    bytes_2_recv = GetSetReportData.Report_Length;
    ControlStage = DATA_OUT_STAGE;
    return true;
#endif
}
#endif

#ifdef USE_GET_IDLE
/**
 * @fn bool HID_GET_IDLE(void)
 * 
 * @brief Returns the Idle Duration and Idle Count.
 * 
 * The Get_Idle request reads the current idle rate for a particular Input report
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool HID_GET_IDLE(void){
#if HID_NUM_IN_REPORTS == 0
    return false;
#else
#if HID_NUM_REPORT_IDS == 0
    if(GetIdleData.Report_ID != 0) return false;
    RAMptr = (uint8_t*)&In_Report_Settings[0].Idle_Duration_4ms;
#else
    if(GetIdleData.Report_ID > HID_NUM_REPORT_IDS) return false;
    if(GetIdleData.Report_ID == 0) return false;
    RAMptr = (uint8_t*)&In_Report_Settings[GetIdleData.Report_ID-1u].Idle_Duration_4ms;
#endif
    if(GetIdleData.wLength != 1) return false;
    send_short = false;
    bytes_2_send = 1;
    SendingFrom = RAM;
    
#if PINGPONG_MODE == PINGPONG_ALL_EP
    EP0_IN_LAST_PPB ^= 1;
    USB_InControlTransfer();
#else
    USB_InControlTransfer();
#endif
    ControlStage = DATA_IN_STAGE;
    
    return true;
#endif
}
#endif

#ifdef USE_SET_IDLE
/**
 * @fn bool HID_SET_IDLE(void)
 * 
 * @brief Modifies the Idle Duration and Idle Count.
 * 
 * The Set_Idle request silences a particular report on the Interrupt In pipe 
 * until a new event occurs or the specified amount of time passes.
 * 
 * @return Returns success (true) or failure (false) to execute the Request.
 */
bool HID_SET_IDLE(void){
#if HID_NUM_IN_REPORTS == 0
    return false;
#else
#if HID_NUM_REPORT_IDS == 0
    if(SetIdleData.Report_ID != 0) return false;
	In_Report_Settings[0].Idle_Count_Overflow = false;
	In_Report_Settings[0].Idle_Count = 0;
    In_Report_Settings[0].Idle_Duration_4ms = SetIdleData.Duration;
    In_Report_Settings[0].Idle_Duration_1ms = ((uint16_t)SetIdleData.Duration)<<2;
#else
    if(SetIdleData.Report_ID > HID_NUM_REPORT_IDS) return false;
    if(SetIdleData.Report_ID == 0){ // All idles are set
        for(uint8_t i=0;i<HID_NUM_IN_REPORTS;i++){
			In_Report_Settings[i].Idle_Count_Overflow = false;
			In_Report_Settings[i].Idle_Count = 0;
            In_Report_Settings[i].Idle_Duration_4ms = SetIdleData.Duration;
            In_Report_Settings[i].Idle_Duration_1ms = ((uint16_t)SetIdleData.Duration)<<2;
        }
    }
    else{
		In_Report_Settings[SetIdleData.Report_ID-1u].Idle_Count_Overflow = false;
		In_Report_Settings[SetIdleData.Report_ID-1u].Idle_Count = 0;
        In_Report_Settings[SetIdleData.Report_ID-1u].Idle_Duration_4ms = SetIdleData.Duration;
        In_Report_Settings[SetIdleData.Report_ID-1u].Idle_Duration_1ms = ((uint16_t)SetIdleData.Duration)<<2;
    }
#endif
    ControlStage = STATUS_IN_STAGE;
    USB_ArmINStatus();
    return true;
#endif
}
#endif

/**
 * @fn void HID_SendReport(uint8_t ReportNum)
 * 
 * @brief Sends OUT Report on HID EP.
 * 
 * @param ReportNum Report number to send.
 */
void HID_SendReport(uint8_t ReportNum){
    if(ReportSent){
#if PINGPONG_MODE == PINGPONG_1_15 || PINGPONG_MODE == PINGPONG_ALL_EP
        if(HID_EP_IN_LAST_PPB == ODD){
            USB_RAMCopy((uint8_t*)In_Report_Ptr[ReportNum], (uint8_t*)HID_EP_IN_EVEN_BUFFER_BASE_ADDR, In_Report_Size[ReportNum]);
            HID_Arm_EP_IN(HID_BD_IN_EVEN,In_Report_Size[ReportNum]);
        }
        else{
            USB_RAMCopy((uint8_t*)In_Report_Ptr[ReportNum], (uint8_t*)HID_EP_IN_ODD_BUFFER_BASE_ADDR, In_Report_Size[ReportNum]);
            HID_Arm_EP_IN(HID_BD_IN_ODD,In_Report_Size[ReportNum]);
        }
#else
        USB_RAMCopy((uint8_t*)In_Report_Ptr[ReportNum], (uint8_t*)HID_EP_IN_BUFFER_BASE_ADDR, In_Report_Size[ReportNum]);
        HID_Arm_EP_IN(In_Report_Size[ReportNum]);
#endif
		USB_Interrupt_Enable = 0;
        In_Report_Settings[ReportNum].Idle_Count = 0;
		USB_Interrupt_Enable = 1;
		SentReportFlag[ReportNum] = false;
		ReportNumSent = ReportNum;
        ReportSent = false;
    }
}

/**
 * @fn void HID_ServiceSOF(void)
 * 
 * @brief Sends OUT Report on HID EP regularly using SOF.
 */
void HID_ServiceSOF(void){
    if(USB_STATE == Configured){
#if HID_NUM_IN_REPORTS > 1
        uint8_t i;
        for(i=0;i<HID_NUM_IN_REPORTS;i++){
            if(In_Report_Settings[i].Idle_Duration_1ms != 0) In_Report_Settings[i].Idle_Count++; // If the Idle_Duration for a report is not infinite (0), increment their counter.
        }
        
        for(i=0;i<HID_NUM_IN_REPORTS;i++){ // If an Idle_Counter is not infinite and goes past the Idle_Duration, send the report.
            if(In_Report_Settings[i].Idle_Duration_1ms != 0 && In_Report_Settings[i].Idle_Count>In_Report_Settings[i].Idle_Duration_1ms-1) In_Report_Settings[i].Idle_Count_Overflow = true;
        }
#else
		if(In_Report_Settings[0].Idle_Duration_1ms != 0){
			In_Report_Settings[0].Idle_Count++; // If the Idle_Duration is not infinite (0), increment the counter.
			if(In_Report_Settings[0].Idle_Count>In_Report_Settings[0].Idle_Duration_1ms-1) In_Report_Settings[0].Idle_Count_Overflow = true;
        }
#endif
    }
}

/**
 * @fn void HID_SetSentReportFlag(void)
 * 
 * @brief Lets the application know that a report has been sent.
 */
void HID_SetSentReportFlag(void){
    ReportSent = true;
    SentReportFlag[ReportNumSent] = true;
}

/**
 * @fn void HID_ClearEndpointToggle(void)
 * 
 * @brief Used to clear HID EP's Data Toggle values.
 */
void HID_ClearEndpointToggle(void){
    EPn_Stat.Array[HID_EP][OUT].Data_Toggle_Val = 0;
    EPn_Stat.Array[HID_EP][IN].Data_Toggle_Val = 0;
}