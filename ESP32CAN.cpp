#include "ESP32CAN.h"

int ESP32CAN::CANInit()
{
    return CAN_init();
}
int ESP32CAN::CANWriteFrame(const CAN_frame_t* p_frame)
{
    return CAN_write_frame(p_frame);
}

int ESP32CAN::CANReadFrame(const CAN_frame_t* p_frame)
{
    return CAN_write_frame(p_frame);
}

int ESP32CAN::CANStop()
{
    return CAN_stop();
}
void ESP32CAN::CANError(int *TXErrors, int *RXErrors)
{
    CAN_error(TXErrors, RXErrors);
}

ESP32CAN ESP32Can;
