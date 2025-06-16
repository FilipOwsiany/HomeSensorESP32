#include "CHardware.h"

#include "CLogger.h"

#include "driver/gpio.h"

CHardware::CHardware(IAdc& aItsAdc, IBme280& aItsBme280) : 
        CBaseTask("CHardware", 4096, 10, this, CHardware::taskFunction),
        mItsBme280(aItsBme280),
        mItsAdc(aItsAdc)
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "CHardware()");
    if (taskInit() == false) 
    {
        CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "taskInit() failed");
        return;
    }

    if (taskRun() == false) 
    {
        CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "taskRun() failed");
        return;
    }

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
}

CHardware::~CHardware()
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "~CHardware()");
}

void CHardware::sendEvent(SEvent& event, bool selfNotify)
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "sendEvent()");
    if (selfNotify) 
    {
        onEvent(event);
    } 
    else 
    {
        notify(event);
    }
}

void CHardware::onEvent(SEvent& event)
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "onEvent()");
    if (event.mEventId == CommonEventId::None) 
    {
        CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "onEvent(): Invalid event data");
        return;
    }
    queueSend(event, portMAX_DELAY);
    taskResume();
}

void CHardware::parseEvent(SEvent& event)
{
    CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Success, "parseEvent() %d", event.mEventId);
    switch (event.mEventId)
    {
    case CommonEventId::ReadAdcRequest:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::ImportantInfo, "parseEvent(): Read ADC value");

            SEvent eventReadAdcACK(CommonEventId::ReadAdcACK);
            sendEvent(eventReadAdcACK, false);

            SEvent eventSendAdcRequest(CommonEventId::SendAdcRequest);
            sendEvent(eventSendAdcRequest, true);
            break;
        }
    case CommonEventId::SendAdcRequest:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::ImportantInfo, "parseEvent(): Send ADC request");

            gpio_set_level(GPIO_NUM_4, 1);
            vTaskDelay(50);
            adcVoltage voltage = mItsAdc.readAvrage(100);
            gpio_set_level(GPIO_NUM_4, 0);
            vTaskDelay(50);

            SEvent eventSendAdcRequest(CommonEventId::SendAdcRequest, sizeof(adcVoltage), 0, static_cast<void*>(&voltage));
            sendEvent(eventSendAdcRequest, false);
            break;
        }
    case CommonEventId::SendAdcACK:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::ImportantInfo, "parseEvent(): Send ADC ACK");
            break;
        }
    case CommonEventId::ReadBme280Request:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::ImportantInfo, "parseEvent(): Read BME280 sensor data");
            SEvent eventReadBme280ACK(CommonEventId::ReadBme280ACK);
            sendEvent(eventReadBme280ACK, false);
            SEvent eventSendBme280Request(CommonEventId::SendBme280Request);
            sendEvent(eventSendBme280Request, true);
            break;
        }
    case CommonEventId::SendBme280Request:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::ImportantInfo, "parseEvent(): Read BME280 sensor data");
            SBme280 bme280Data(0.0f, 0.0f, 0.0f);
            mItsBme280.read(bme280Data);

            SEvent eventSendBme280Request(CommonEventId::SendBme280Request, sizeof(SBme280), 0, static_cast<void*>(&bme280Data));
            sendEvent(eventSendBme280Request, false);
            break;
        }
    case CommonEventId::SendBme280ACK:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::ImportantInfo, "parseEvent(): Send BME280 ACK");
            break;
        }
    default:
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Warning, "parseEvent(): Unknown event ID: %u", event.mEventId);
            break;
        }
    }

    if (event.mDataSize > 0 && event.mData != nullptr) 
    {
        free(event.mData);
    }
}

void CHardware::taskFunction(void* pvParameter)
{
    CHardware* self = static_cast<CHardware*>(pvParameter);

    // SEvent event(1, sizeof(uint32_t), 2, new uint32_t(1234));
    // while (1) 
    // {
    //     self->taskDelay(100);

    //     static uint32_t testData = 0;
    //     testData++;
    //     SEvent event(1, sizeof(uint32_t), 2, static_cast<void*>(&testData));
    //     self->sendEvent(event);

    //     self->taskDelay(1000);

    //     CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "CHardware::taskFunction() free heap [%lu] minimum ever [%lu] largest block [%u]", 
    //         esp_get_free_heap_size(), esp_get_minimum_free_heap_size(), heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    // }

    while (1) 
    {
        SEvent event;
        if (self->queueReceive(event, portMAX_DELAY) == true) 
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "taskFunction(): Received message, size: %ld", event.mDataSize);
            if (event.mEventId == CommonEventId::None && event.mDataSize == 0 && event.mData == nullptr && event.mTime == 0) 
            {
                CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "taskFunction(): Invalid event data");
                continue;
            }
            self->parseEvent(event);
        }
        else 
        {
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Error, "taskFunction(): Failed to receive message");
        }

        if (self->queueGetSize() == 0) 
        {
            self->taskSuspend();
            CLogger::log(CLoggerModule::Hardware, CLoggerLevel::Debug, "taskFunction(): No messages in queue, suspending task");
        }
    }
}