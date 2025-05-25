#include "CBaseQueue.h"

#include "CLogger.h"

CBaseQueue::CBaseQueue(uint8_t queueSize)
{
    CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Debug, "CBaseQueue::CBaseQueue()");
    mQueue = xQueueCreate(queueSize, sizeof(CMessageQueue));
    if (mQueue == NULL) 
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Failed to create queue");
    }
}

CBaseQueue::~CBaseQueue()
{
    if (mQueue != NULL) 
    {
        vQueueDelete(mQueue);
        mQueue = NULL;
    }
    else 
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Warning, "CBaseQueue::~CBaseQueue(): Queue is not created");
    }
}

bool CBaseQueue::queueSend(SEvent& event, TickType_t waitTime)
{
    CMessageQueue message = CMessageQueue(event);
    if(message.mEvent.mEventId == 0 && message.mEvent.mDataSize == 0 && message.mEvent.mData == NULL && message.mEvent.mTime == 0) 
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Invalid event data");
        return false;
    } 
    BaseType_t res = xQueueSend(mQueue, &message, waitTime);
    if (res != pdPASS) 
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Failed to send message to queue");
        return false;
    }
    return true;
}

bool CBaseQueue::queueReceive(SEvent& event, TickType_t waitTime)
{
    CMessageQueue message;
    if (mQueue == NULL) 
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Queue is not created");
        return false;
    }
    BaseType_t res = xQueueReceive(mQueue, &message, waitTime);
    if (res != pdPASS) 
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Failed to receive message from queue");
        return false;
    }

    event.mEventId = message.mEvent.mEventId;
    event.mDataSize = message.mEvent.mDataSize;
    event.mTime = message.mEvent.mTime;
    event.mData = message.mEvent.mData;
    return true;
}

uint8_t CBaseQueue::queueGetSize(void) 
{ 
    return uxQueueMessagesWaiting(mQueue); 
}

uint8_t CBaseQueue::queueGetFreeSize(void)
{ 
    return uxQueueSpacesAvailable(mQueue); 
}