#include "CHardware.h"

#include "CLogger.h"

CHardware::CHardware() : CBaseTask("CHardware", 4096, 10, this, CHardware::taskFunction)
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "CHardware::CHardware()");
    if (taskInit() == false) 
    {
        CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "CHardware::taskInit() failed");
        return;
    }
    if (taskRun() == false) 
    {
        CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "CHardware::taskRun() failed");
        return;
    }
}

CHardware::~CHardware()
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "CHardware::~CHardware()");
}

void CHardware::sendEvent(SEvent& event)
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "CHardware::sendEvent()");
    notify(event);
}

void CHardware::taskFunction(void* pvParameter)
{
    CHardware* self = static_cast<CHardware*>(pvParameter);

    SEvent event(1, sizeof(uint32_t), 2, new uint32_t(1234));
    while (1) 
    {
        self->taskDelay(100);

        static uint32_t testData = 0;
        testData++;
        SEvent event(1, sizeof(uint32_t), 2, static_cast<void*>(&testData));
        self->sendEvent(event);

        self->taskDelay(100);

        CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "CHardware::taskFunction() free heap [%lu] minimum ever [%lu] largest block [%u]", 
            esp_get_free_heap_size(), esp_get_minimum_free_heap_size(), heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    }
}