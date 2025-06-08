#ifndef __CCONTROL_H__
#define __CCONTROL_H__

#include "commonStd.h"

#include "IEventListener.h"
#include "IEventPublisherManager.h"
#include "CBaseTask.h"
#include "CBme280.h"
#include "CAdc.h"

#include "IWifi.h"

class CControl : public IEventListener, public IEventPublisherManager, public CBaseTask
{
private:
    std::vector<IEventListener*> listeners;
    IWifi * wifiInstance = nullptr;
    CBme280* bme280Sensor = nullptr;
    CAdc* adc = nullptr;
public:
    CControl();
    ~CControl();

    void sendEvent(SEvent& event) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CCONTROL_H__