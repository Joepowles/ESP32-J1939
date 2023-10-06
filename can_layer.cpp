// ------------------------------------------------------------------------------------------------------
// Project: JCOM_J1939_ARD
// File:    can.cpp
// ------------------------------------------------------------------------------------------------------
#include "ESP32CAN.h"
#include "CAN_config.h"

#define OK                                    0
#define ERR                                   1

// Module variables -------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
CAN_device_t CAN_cfg;
long lCANBaudrate = CAN_SPEED_250KBPS;
#define QUEUELENGTH 20

// ------------------------------------------------------------------------------------------------------
// canInit
// ------------------------------------------------------------------------------------------------------
// lBaudrate = CAN_SPEED_250KBPS/CAN_SPEED_500KBPS
//
unsigned char canInit(long lBaudrate)
{
    // Stop the CAN port
    ESP32Can.CANStop();
    
    // Configure the CAN Module
    CAN_cfg.speed = (CAN_speed_t)lBaudrate;
    CAN_cfg.tx_pin_id = GPIO_NUM_25;
    CAN_cfg.rx_pin_id = GPIO_NUM_26;
    CAN_cfg.rx_queue = xQueueCreate(QUEUELENGTH,sizeof(CAN_frame_t));
    
    // Start the CAN Module
    ESP32Can.CANInit(); // Always returns 0 = OK

    return OK;

}// end canInit

// ------------------------------------------------------------------------------------------------------
// canCheckError
// ------------------------------------------------------------------------------------------------------
// 0 - No Error
// 1 - Errors exist
//
// For the purpose of detecting message ID collisions under SAE J1939, we check only TX errors
//
unsigned char canCheckError(void)
{
    // Declarations
    unsigned char ErrorFlag = 0;
    
    int RXErrors = 0;
    int TXErrors = 0;

    // Check for errors
    ESP32Can.CANError(&TXErrors, &RXErrors);
    if(TXErrors > 0)
        ErrorFlag = 1;

    // Return the result
    return ErrorFlag;
    
}// end canCheckError

// ------------------------------------------------------------------------------------------------------
// canTransmit
// ------------------------------------------------------------------------------------------------------
unsigned char canTransmit(long lID, unsigned char* pData, int nDataLen)
{
    // Declarations
    unsigned char nRetCode = ERR;
    CAN_frame_t tx_frame;

    // Copy all information into the structure
    tx_frame.FIR.B.FF = CAN_frame_ext;
    tx_frame.MsgID = (uint32_t)lID;
    tx_frame.FIR.B.DLC = (uint8_t)nDataLen;

    for(int nIndex = 0; nIndex < nDataLen; nIndex++)
        tx_frame.data.u8[nIndex] = pData[nIndex];

    // Transmit the CAN message frame
    if(ESP32Can.CANWriteFrame(&tx_frame) == 0)
        nRetCode = OK;

    return nRetCode;
    
}// end canTransmit

// ------------------------------------------------------------------------------------------------------
// canReceive
// ------------------------------------------------------------------------------------------------------
unsigned char canReceive(long* lID, unsigned char* pData, int* nDataLen)
{
    // Declarations
    unsigned char nRetCode = ERR;
    CAN_frame_t rx_frame;

    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS) == pdTRUE)
    {
        // Check for extended frame
        if(rx_frame.FIR.B.FF == CAN_frame_ext)
        {
            // Copy the information from the structure
            *lID = rx_frame.MsgID;
            *nDataLen = rx_frame.FIR.B.DLC;

            for(int nIndex = 0; nIndex < *nDataLen; nIndex++)
                pData[nIndex] = rx_frame.data.u8[nIndex];

            nRetCode = OK;

        }// end if

    }// end if

    return nRetCode;
    
}// end canTransmit

// end can.cpp
