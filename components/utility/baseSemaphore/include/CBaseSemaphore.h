#ifndef __CBASEMUTEX_H__
#define __CBASEMUTEX_H__

#include "commonStd.h"
#include "commonOs.h"

class CBaseSemaphore
{
private:
    SemaphoreHandle_t mSemaphore;
public:
    CBaseSemaphore();
    ~CBaseSemaphore();
    bool take(TickType_t blockTime = portMAX_DELAY);
    bool takeFromISR(void);
    bool give(void);
    bool giveFromISR(void);
};

#endif //__CBASEMUTEX_H__