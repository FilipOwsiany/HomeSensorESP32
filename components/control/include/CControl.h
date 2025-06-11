#ifndef __CCONTROL_H__
#define __CCONTROL_H__

#include "commonStd.h"
#include "commonEvent.h"

#include "IEventListener.h"
#include "IEventPublisherManager.h"
#include "CBaseTask.h"

#include "SBme280.h"

#include "IWifi.h"

class CControl : public IEventListener, public IEventPublisherManager, public CBaseTask
{
private:
    std::vector<IEventListener*> listeners;
    IWifi * wifiInstance = nullptr;

    void parseEvent(SEvent& event);
public:
    CControl();
    ~CControl();

    void sendEvent(SEvent& event, bool selfNotify) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CCONTROL_H__