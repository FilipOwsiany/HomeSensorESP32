#ifndef __CCCOMUNICATION_H__
#define __CCCOMUNICATION_H__

#include "commonStd.h"
#include "commonEvent.h"

#include "EventHelper.h"
#include "CBaseTask.h"

#include "IHttpsClient.h"

class CCommunication : public IEventListener, public IEventPublisherManager, public CBaseTask
{
private:
    IHttpsClient& mItsHttpsClient;
    CLoggerHelper mLog;

    void parseEvent(SEvent& event);
    friend void processQueueEvent<CCommunication>(CCommunication& self, CLoggerModule loggerModule);
public:
    CCommunication();
    ~CCommunication();

    void sendEvent(SEvent& event, bool selfNotify) override;
    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
    static void httpsResponseHandler(SHttpClientRequest& event);
};

#endif //__CCCOMUNICATION_H__