#ifndef __CCCOMUNICATION_H__
#define __CCCOMUNICATION_H__

#include "commonStd.h"
#include "commonEvent.h"

#include "IEventListener.h"
#include "IEventPublisherManager.h"
#include "CBaseTask.h"

class CCommunication : public IEventListener, public IEventPublisherManager, public CBaseTask
{
private:
    void parseEvent(SEvent& event);
public:
    CCommunication();
    ~CCommunication();

    void sendEvent(SEvent& event, bool selfNotify) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CCCOMUNICATION_H__