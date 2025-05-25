#ifndef __IEVENTLISTENER_H__
#define __IEVENTLISTENER_H__

#include "commonStd.h"

#include "SEvent.h"
#include "CBaseQueue.h"

class IEventListener : public CBaseQueue
{

public:

    IEventListener(uint8_t queueSize = 10) : CBaseQueue(queueSize) {}
    ~IEventListener() = default;
    
    virtual void onEvent(SEvent& event) = 0;
};

#endif //__IEVENTLISTENER_H__