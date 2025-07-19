#ifndef __CHARDWARE_H__
#define __CHARDWARE_H__

#include "commonStd.h"
#include "commonEvent.h"

#include "EventHelper.h"
#include "CLoggerHelper.h"

#include "CBaseTask.h"

#include "IBme280.h"
#include "IAdc.h"
#include "IStorage.h"

#include "CMeasQueue.h"

class CHardware : public IEventListener, public IEventPublisherManager, public CBaseTask
{
private:
    IBme280& mItsBme280;
    IAdc& mItsAdc;
    IStorage& mItsStorage;

    CMeasQueue mMeasQueue;

    CLoggerHelper mLog;

    void parseEvent(SEvent& event);
    friend void processQueueEvent<CHardware>(CHardware& self, CLoggerModule loggerModule);
public:
    CHardware(IBme280& aItsBme280, IAdc& aItsAdc, IStorage& aItsStorage);
    ~CHardware();

    void sendEvent(SEvent& event, bool selfNotify) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CHARDWARE_H__