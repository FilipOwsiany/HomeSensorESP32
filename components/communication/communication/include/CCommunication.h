#ifndef __CCCOMUNICATION_H__
#define __CCCOMUNICATION_H__

#include "commonStd.h"

#include "IEventListener.h"
#include "CBaseTask.h"

class CCommunication : public IEventListener, public CBaseTask
{
private:
public:
    CCommunication();
    ~CCommunication();

    void onEvent(SEvent& event) override;
    static void taskFunction(void* pvParameter);
    //void parseEvent(void* eventData, uint32_t dataSize);
};

#endif //__CCCOMUNICATION_H__