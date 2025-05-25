#ifndef __CBASEQUEUE_H__
#define __CBASEQUEUE_H__

#include "commonStd.h"
#include "commonOs.h"

#include "SEvent.h"
#include "CMessageQueue.h"

typedef void* QueueMessage_t;

class CBaseQueue
{
private:
    QueueHandle_t mQueue;
public:
    CBaseQueue(uint8_t queueSize);
    ~CBaseQueue();

    bool queueSend(SEvent& event, TickType_t waitTime = portMAX_DELAY);
    bool queueReceive(SEvent& event, TickType_t waitTime = portMAX_DELAY);
    uint8_t queueGetSize(void);
    uint8_t queueGetFreeSize(void);

};

#endif //__CBASEQUEUE_H__