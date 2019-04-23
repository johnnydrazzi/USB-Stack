
#include "USB.h"
#include "usb_cdc.h"
#include "usb_hal.h"
#include "usb_ch9.h"

uint8_t DummyBuffer[8] = {0};

/**
 * @fn void CDC_Arm_COM_EP_IN(void)
 * 
 * @brief Arms CDC COM Endpoint for a transaction.
 * 
 * The function is used to arm CDC COM EP for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * CDC_Arm_COM_EP_IN();
 * @endcode
 * </li></ul>
 */
void CDC_Arm_COM_EP_IN(void){
    if(CDC_COM_EP_IN_DATA_TOGGLE_VAL) BDT.Array[CDC_COM_BD_IN].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[CDC_COM_BD_IN].STAT = _DTSEN; // DATA0
    BDT.Array[CDC_COM_BD_IN].CNT = 10;
    BDT.Array[CDC_COM_BD_IN].STAT |= _UOWN;
}

/**
 * @fn CDC_Arm_DAT_EP_OUT(void)
 * 
 * @brief Arms CDC DAT EP OUT Endpoint for a transaction.
 * 
 * The function is used to arm CDC DAT EP OUT for a transaction.
 * 
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * CDC_Arm_DAT_EP_OUT();
 * @endcode
 * </li></ul>
 */
void CDC_Arm_DAT_EP_OUT(void){
    if(CDC_DAT_EP_OUT_DATA_TOGGLE_VAL) BDT.Array[CDC_DAT_BD_OUT].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[CDC_DAT_BD_OUT].STAT = _DTSEN; // DATA0
    BDT.Array[CDC_DAT_BD_OUT].CNT = CDC_DAT_EP_SIZE;
    BDT.Array[CDC_DAT_BD_OUT].STAT |= _UOWN;
}

/**
 * @fn void CDC_Arm_DAT_EP_IN(uint16_t cnt)
 * 
 * @brief Arms CDC DAT EP IN Endpoint for a transaction.
 * 
 * The function is used to arm CDC DAT EP IN for a transaction.
 * 
 * @param[in] cnt Amount of bytes being transfered.
 * 
 * <b>Code Example:</b>
 * <ul style="list-style-type:none"><li>
 * @code
 * CDC_Arm_DAT_EP_IN(CDC_DAT_EP_SIZE);
 * @endcode
 * </li></ul>
 */
void CDC_Arm_DAT_EP_IN(uint16_t cnt){
    if(CDC_DAT_EP_IN_DATA_TOGGLE_VAL) BDT.Array[CDC_DAT_BD_IN].STAT = _DTSEN | _DTS; // DATA1
    else BDT.Array[CDC_DAT_BD_IN].STAT = _DTSEN; // DATA0
    BDT.Array[CDC_DAT_BD_IN].CNT = cnt;
    BDT.Array[CDC_DAT_BD_IN].STAT |= _UOWN;
}

bool CDC_ClassRequest(void){
    switch(SetupData.bRequest){
#ifdef USE_GET_LINE_CODING
        case GET_LINE_CODING:
            RAMptr = (uint8_t*)&GetLineCodingReturnedData;
            bytes_available = 7;
            if(bytes_available < SetGetLineCodingData.Size_of_Structure){
                bytes_2_send = bytes_available;
                if(SetGetLineCodingData.Size_of_Structure % bytes_available) send_short = true;
                else send_short = false;
            }
            else{
                bytes_2_send = SetGetLineCodingData.Size_of_Structure;
                send_short = false;
            }
            SendingFrom = RAM;
            USB_InControlTransfer();
            ControlStage = DATA_IN_STAGE;
            return true;
#endif
#ifdef USE_SET_LINE_CODING
        case SET_LINE_CODING:
            RAMptr = (uint8_t*)&SetLineCodingData;
            if(SetGetLineCodingData.Size_of_Structure > 7) return false;
            bytes_2_recv = SetGetLineCodingData.Size_of_Structure;
            setLineCodingWait = true;
            ControlStage = DATA_OUT_STAGE;
            return true;
#endif
#ifdef USE_SET_CONTROL_LINE_STATE
        case SET_CONTROL_LINE_STATE:
            if(SetupData.wIndex != 0) return false;
            CDC_SetControlLineState();
            USB_ArmINStatus();
            return true;
#endif
        case SEND_ENCAPSULATED_COMMAND:
            RAMptr = (uint8_t*)DummyBuffer;
            if(SetupData.wLength > 8) return false;
            bytes_2_recv = SetupData.wLength;
            ControlStage = DATA_OUT_STAGE;
            return true;
        case GET_ENCAPSULATED_RESPONSE:
            RAMptr = (uint8_t*)DummyBuffer;
            bytes_available = 8;
            if(bytes_available < SetupData.wLength){
                bytes_2_send = bytes_available;
                if(SetupData.wLength % bytes_available) send_short = true;
                else send_short = false;
            }
            else{
                bytes_2_send = SetupData.wLength;
                send_short = false;
            }
            SendingFrom = RAM;
            USB_InControlTransfer();
            ControlStage = DATA_IN_STAGE;
            return true;
        default:
            return false;
    }
    

}

void CDC_Init(void){
#ifdef USE_GET_LINE_CODING
    GetLineCodingReturnedData.dwDTERate   = STARTING_BAUD;
    GetLineCodingReturnedData.bCharFormat = STARTING_STOP_BITS;
    GetLineCodingReturnedData.bParityType = PARITY_NONE;
    GetLineCodingReturnedData.bDataBits   = STARTING_DATA_BITS;
#endif
    
#ifdef USE_RTS
    RTS = RTS_ACTIVE ^ 1;
    RTS_TRIS = 0;
#endif
#ifdef USE_DTR
    DTR = DTR_ACTIVE ^ 1;
    DTR_TRIS = 0;
#endif
    
#if defined(USE_DTR)||defined(USE_DCD)
    USB_RAMSet(0,SERIAL_STATE_Data.Array,10);
    SERIAL_STATE_Data.Header.bmRequestType = 0xA1;
    SERIAL_STATE_Data.Header.bNotification = SERIAL_STATE;
    SERIAL_STATE_Data.Header.wValue = 0;
    SERIAL_STATE_Data.Header.wIndex = 1;
    SERIAL_STATE_Data.Header.wLength = 2;
#ifdef USE_DCD
    SERIAL_STATE_Data.bRxCarrier = (DCD ^ DCD_ACTIVE) ^ 1; // DCD
#else
    SERIAL_STATE_Data.bRxCarrier = 1; // DCD
#endif
#ifdef USE_DTR
    SERIAL_STATE_Data.bTxCarrier = (DSR ^ DSR_ACTIVE) ^ 1; // DSR
#else
    SERIAL_STATE_Data.bTxCarrier = 1; // DSR
#endif
#endif

    // BD settings
    BDT.Array[CDC_COM_BD_IN].STAT = 0;
    BDT.Array[CDC_COM_BD_IN].ADR = CDC_COM_EP_IN_BUFFER_BASE_ADDR;
    BDT.Array[CDC_DAT_BD_OUT].STAT = 0;
    BDT.Array[CDC_DAT_BD_OUT].ADR = CDC_DAT_EP_OUT_BUFFER_BASE_ADDR;
    BDT.Array[CDC_DAT_BD_IN].STAT = 0;
    BDT.Array[CDC_DAT_BD_IN].ADR = CDC_DAT_EP_IN_BUFFER_BASE_ADDR;
    
    // EP Settings
    CDC_COM_UEPbits.EPHSHK = 1;     // Handshaking enabled 
    CDC_COM_UEPbits.EPINEN = 1;     // EP input enabled
    CDC_DAT_UEPbits.EPHSHK = 1;     // Handshaking enabled 
    CDC_DAT_UEPbits.EPCONDIS = 0;   // Don't allow SETUP
    CDC_DAT_UEPbits.EPOUTEN = 1;    // EP output enabled
    CDC_DAT_UEPbits.EPINEN = 1;     // EP input enabled
    
    EPn_Stat.Array[CDC_COM_EP][IN].Halt = 0;
    EPn_Stat.Array[CDC_DAT_EP][OUT].Halt = 0;
    EPn_Stat.Array[CDC_DAT_EP][IN].Halt = 0;
    CDC_ClearEndpointToggle();
    CDC_Arm_DAT_EP_OUT();
    
#if defined(USE_DTR)||defined(USE_DCD)
    CDC_Arm_COM_EP_IN();
#endif
    setLineCodingWait = false;
}

void CDC_ClearEndpointToggle(void){
    CDC_COM_EP_IN_DATA_TOGGLE_VAL = 0;
    CDC_DAT_EP_OUT_DATA_TOGGLE_VAL = 0;
    CDC_DAT_EP_IN_DATA_TOGGLE_VAL = 0;
}

void CDC_Tasks(void){
    if(TRANSACTION_EP == CDC_COM_EP){
        CDC_COM_EP_IN_DATA_TOGGLE_VAL ^= 1;
        CDC_Notification();
    }
    else if(TRANSACTION_EP == CDC_DAT_EP){
        if(TRANSACTION_DIR == OUT){
            CDC_DAT_EP_OUT_DATA_TOGGLE_VAL ^= 1;
            NumDataOut = BDT.Array[CDC_DAT_BD_OUT].CNT;
            CDC_DataOUT();
        }
        else{
            CDC_DAT_EP_IN_DATA_TOGGLE_VAL ^= 1;
            CDC_DataIN();
        }
    }
}

bool CDC_OutControlTasks(void){
#ifdef USE_GET_LINE_CODING
    if(setLineCodingWait){
        setLineCodingWait = false;
        if(SetLineCodingData.bCharFormat != 0) return false;
        if(SetLineCodingData.bParityType != 0) return false;
        if(SetLineCodingData.bDataBits != 8) return false;
        
        GetLineCodingReturnedData.dwDTERate = SetLineCodingData.dwDTERate;
        GetLineCodingReturnedData.bCharFormat = SetLineCodingData.bCharFormat;
        GetLineCodingReturnedData.bParityType = SetLineCodingData.bParityType;
        GetLineCodingReturnedData.bDataBits = SetLineCodingData.bDataBits;
        
        CDC_SetLineCoding();
        return true;
    }
#endif
    return false;
}

#if defined(USE_DTR)||defined(USE_DCD)
void NotificationTasks(void){
    if(sentLastNotification && sendNotification){
        sendNotification = false;
        sentLastNotification = false;
        CDC_Arm_COM_EP_IN();
    }
}
#endif