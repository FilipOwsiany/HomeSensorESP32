#ifndef __IEVENTPUBLISHER_H__
#define __IEVENTPUBLISHER_H__

#include "commonStd.h"

#include "SEvent.h"
#include "IEventListener.h"

class IEventPublisher
{

public:
    virtual ~IEventPublisher() = default;

    virtual void subscribe(IEventListener* listener) = 0;
    virtual void unsubscribe(IEventListener* listener) = 0;
    virtual void notify(SEvent& event) = 0;

};

#endif //__IEVENTPUBLISHER_H__