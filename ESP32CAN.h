#ifndef ESP32CAN_H
#define ESP32CAN_H

#include "CAN_config.h"
#include "CAN.h"

class ESP32CAN
{
    public: 
        int CANInit();
        int CANWriteFrame(const CAN_frame_t* p_frame);
		int CANReadFrame(const CAN_frame_t* p_frame);
        int CANStop();
        void CANError(int *TXErrors, int *RXErrors);

};

extern ESP32CAN ESP32Can;
#endif
