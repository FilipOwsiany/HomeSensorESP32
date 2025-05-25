#ifndef __CHARDWARE_H__
#define __CHARDWARE_H__

#include "commonStd.h"

#include "IEventPublisherManager.h"
#include "CBaseTask.h"

class CHardware : public IEventPublisherManager, public CBaseTask
{
public:
    CHardware();
    ~CHardware();

    void sendEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
};

#endif //__CHARDWARE_H__