#ifndef __SEVENT_H__
#define __SEVENT_H__

#include "commonStd.h"

struct SEvent
{
    uint32_t mEventId;
    uint32_t mDataSize;
    uint32_t mTime;
    void* mData;

    SEvent() : mEventId(0), mDataSize(0), mTime(0), mData(nullptr) {}
    SEvent(uint32_t eventId, uint32_t dataSize, uint32_t time, void* data)
        : mEventId(eventId), mDataSize(dataSize), mTime(time), mData(data) {}
};

#endif //__SEVENT_H__