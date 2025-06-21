#ifndef __EVENTHELPER_H__
#define __EVENTHELPER_H__

#include "commonStd.h"

#include "IEventListener.h"
#include "IEventPublisherManager.h"

#include "SEvent.h"
#include "CLogger.h"
#include "CLoggerHelper.h"

template <typename T>
void processQueueEvent(T& self, CLoggerModule loggerModule)
{
    SEvent event;
    if (self.queueReceive(event, portMAX_DELAY) == true)
    {
        CLogger::log(loggerModule, CLoggerLevel::Debug,
                     "taskFunction(): Received message, size: %ld",
                     event.mDataSize);

        if (event.mEventId == CommonEventId::None &&
            event.mDataSize == 0 &&
            event.mData == nullptr &&
            event.mTime == 0)
        {
            CLogger::log(loggerModule, CLoggerLevel::Error,
                         "taskFunction(): Invalid event data");
            return;
        }
        self.parseEvent(event);
    }
    else
    {
        CLogger::log(loggerModule, CLoggerLevel::Error,
                     "taskFunction(): Failed to receive message");
    }

    if (self.queueGetSize() == 0)
    {
        self.taskSuspend();
        CLogger::log(loggerModule, CLoggerLevel::Debug,
                     "taskFunction(): No messages in queue, suspending task");
    }
}

#endif //__EVENTHELPER_H__