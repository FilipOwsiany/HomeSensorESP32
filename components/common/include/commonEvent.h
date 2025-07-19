#ifndef __COMMONEVENT_H__
#define __COMMONEVENT_H__

#include "commonStd.h"

enum class CommonEventId : uint32_t
{
    None = 0,
    ApplicationStart = 1,
    ReadMeasRequest,
    ReadMeasACK,
    SendMeasRequest,
    SendMeasACK,

    ReadAdcRequest,
    ReadAdcACK,
    SendAdcRequest,
    SendAdcACK,
    ReadBme280Request,
    ReadBme280ACK,
    SendBme280Request,
    SendBme280ACK,
    SendDataToServer, 
    dummy,
    SendDataToServerMultiple,
    ApplicationStop,
};

#endif //__COMMONEVENT_H__