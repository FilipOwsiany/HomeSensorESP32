#ifndef __CCONTROL_H__
#define __CCONTROL_H__

#include "commonStd.h"
#include "commonEvent.h"
#include "commonData.h"

#include "EventHelper.h"
#include "CBaseTask.h"

#include "SBme280.h"

#include "IWifi.h"
#include "CLowPower.h"

class CControl : public IEventListener, public IEventPublisherManager, public CBaseTask
{
private:

    std::vector<IEventListener*> listeners;
    IWifi& mItsWifi;
    CLowPower& mItsLowPower;

    CLoggerHelper mLog;
    SMeassurment mMeasurement;

    void parseEvent(SEvent& event);
    friend void processQueueEvent<CControl>(CControl& self, CLoggerModule loggerModule);
public:
    CControl(CLowPower& aItsLowPower);
    ~CControl();

    void sendEvent(SEvent& event, bool selfNotify) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CCONTROL_H__