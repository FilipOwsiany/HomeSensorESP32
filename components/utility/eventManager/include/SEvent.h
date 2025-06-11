#ifndef __SEVENT_H__
#define __SEVENT_H__

#include "commonStd.h"
#include "commonEvent.h"

struct SEvent
{
    CommonEventId mEventId;
    uint32_t mDataSize;
    uint32_t mTime;
    void* mData;

    SEvent() : mEventId(CommonEventId::None), mDataSize(0), mTime(0), mData(nullptr) {}
    SEvent(CommonEventId eventId) : mEventId(eventId), mDataSize(0), mTime(0), mData(nullptr) {}
    SEvent(CommonEventId eventId, uint32_t dataSize, uint32_t time, void* data)
        : mEventId(eventId), mDataSize(dataSize), mTime(time), mData(data) {}
};

#endif //__SEVENT_H__