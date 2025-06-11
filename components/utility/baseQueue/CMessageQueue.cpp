#include "CMessageQueue.h"

#include "CLogger.h"

CMessageQueue::CMessageQueue()
{
    CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Debug, "CMessageQueue::CMessageQueue()");
    mEvent.mEventId = CommonEventId::None;
    mEvent.mDataSize = 0;
    mEvent.mData = nullptr;
    mEvent.mTime = 0;
}

CMessageQueue::CMessageQueue(SEvent& event)
{
    CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Debug, "CMessageQueue::CMessageQueue()");

    if (event.mDataSize == 0 || event.mData == nullptr) 
    {
        mEvent.mEventId = event.mEventId;
        mEvent.mDataSize = event.mDataSize;
        mEvent.mTime = event.mTime;
        mEvent.mData = nullptr;

    }
    else
    {
        mEvent.mData = static_cast<void*>(malloc(event.mDataSize));
        if (mEvent.mData != nullptr) 
        {
            mEvent.mEventId = event.mEventId;
            mEvent.mDataSize = event.mDataSize;
            mEvent.mTime = event.mTime;
            memcpy(mEvent.mData, event.mData, event.mDataSize);
        }
        else 
        {
            CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Failed to allocate memory for message data");
            mEvent.mEventId = CommonEventId::None;
            mEvent.mDataSize = 0;
            mEvent.mData = nullptr;
            mEvent.mTime = 0;
        }
    }
}

CMessageQueue::~CMessageQueue()
{
    CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Debug, "CMessageQueue::~CMessageQueue()");
    mEvent.mEventId = CommonEventId::None;
    mEvent.mDataSize = 0;
    mEvent.mData = nullptr;
    mEvent.mTime = 0;
}