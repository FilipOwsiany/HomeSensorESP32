#include "CBaseSemaphore.h"

#include "CLogger.h"

CBaseSemaphore::CBaseSemaphore()
{
    mSemaphore = xSemaphoreCreateMutex();
    if (mSemaphore == NULL)
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Failed to create semaphore");
    }
}

CBaseSemaphore::~CBaseSemaphore()
{
    if (mSemaphore != NULL)
    {
        vSemaphoreDelete(mSemaphore);
        mSemaphore = NULL;
    }
    else
    {
        CLogger::log(CLoggerModule::BaseQueue, CLoggerLevel::Error, "Semaphore already deleted or not initialized");
    }
}

bool CBaseSemaphore::take(TickType_t blockTime)
{
    return xSemaphoreTake(mSemaphore, blockTime) == pdTRUE;
}

bool CBaseSemaphore::takeFromISR(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    bool result = xSemaphoreTakeFromISR(mSemaphore, &xHigherPriorityTaskWoken) == pdTRUE;

    // If a higher priority task was woken, yield to it
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }

    return result;
}

bool CBaseSemaphore::give(void)
{
    return xSemaphoreGive(mSemaphore) == pdTRUE;
}

bool CBaseSemaphore::giveFromISR(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    bool result = xSemaphoreGiveFromISR(mSemaphore, &xHigherPriorityTaskWoken) == pdTRUE;

    // If a higher priority task was woken, yield to it
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }

    return result;
}