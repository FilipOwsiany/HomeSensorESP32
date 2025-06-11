#ifndef __IEVENTPUBLISHERMANAGER_H__
#define __IEVENTPUBLISHERMANAGER_H__

#include "commonStd.h"

#include "SEvent.h"
#include "IEventListener.h"
#include "IEventPublisher.h"

class IEventPublisherManager : public IEventPublisher
{
private:
    std::vector<IEventListener*> listeners;
public:
    IEventPublisherManager();
    ~IEventPublisherManager();

    void subscribe(IEventListener* listener) override;
    void unsubscribe(IEventListener* listener) override;
    void notify(SEvent& event) override;

    virtual void sendEvent(SEvent& event, bool selfNotify) = 0;
};



#endif //__IEVENTPUBLISHERMANAGER_H__