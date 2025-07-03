#ifndef __CCCOMUNICATION_H__
#define __CCCOMUNICATION_H__

#include "commonStd.h"
#include "commonEvent.h"

#include "EventHelper.h"
#include "CBaseTask.h"

#include "IHttpsClient.h"
#include "IHttpsClientCallback.h"

class CCommunication : public IEventListener, public IEventPublisherManager, public CBaseTask, public IHttpsClientCallback
{
private:
    IHttpsClient* mItsHttpsClient;
    CLoggerHelper mLog;

    void parseEvent(SEvent& event);
    friend void processQueueEvent<CCommunication>(CCommunication& self, CLoggerModule loggerModule);
    void responseCallback(SHttpClientRequest& event) override;
public:
    CCommunication();
    ~CCommunication();

    void sendEvent(SEvent& event, bool selfNotify) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CCCOMUNICATION_H__