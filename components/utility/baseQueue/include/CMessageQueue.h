#ifndef __CMESSAGEQUEUE_H__
#define __CMESSAGEQUEUE_H__

#include "commonStd.h"
#include "SEvent.h"

typedef void* QueueData_t;

class CMessageQueue
{
public:
    CMessageQueue();
    CMessageQueue(SEvent& event);
    ~CMessageQueue();
    SEvent mEvent;
};

#endif //__CMESSAGEQUEUE_H__