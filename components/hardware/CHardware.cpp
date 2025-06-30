#include "CHardware.h"
#include "CHalEspressifFactory.h"
#include "driver/gpio.h"

#include "CBme280.h"

CHardware::CHardware(IBme280& aItsBme280) :
    IEventListener(10), 
    CBaseTask("CHardware", 4096, 10, this, CHardware::taskFunction),
    mItsBme280(aItsBme280),
    mItsAdc(CHalEspressifFactory::createAdc()),
    mLog(CLoggerConfig::CLogModule::Hardware)
{
    mLog.debug("%s", __FUNCTION__);

    if (taskInit() == false) 
    {
        mLog.error("%s: taskInit() failed", __FUNCTION__);
        return;
    }

    if (taskRun() == false) 
    {
        mLog.error("%s: taskRun() failed", __FUNCTION__);
        return;
    }

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
}

CHardware::~CHardware()
{

}

void CHardware::sendEvent(SEvent& event, bool selfNotify)
{
    mLog.debug("%s", __FUNCTION__);
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
    mLog.debug("%s", __FUNCTION__);
    if (event.mEventId == CommonEventId::None) 
    {
        mLog.error("%s: invalid event data", __FUNCTION__);
        return;
    }
    queueSend(event, portMAX_DELAY);
    taskResume();
}

void CHardware::parseEvent(SEvent& event)
{
    mLog.importantInfo("%s: %d", __FUNCTION__, event.mEventId);
    switch (event.mEventId)
    {
    case CommonEventId::ReadAdcRequest:
        {
            mLog.importantInfo("%s: read ADC value", __FUNCTION__);
            SEvent eventReadAdcACK(CommonEventId::ReadAdcACK);
            sendEvent(eventReadAdcACK, false);

            SEvent eventSendAdcRequest(CommonEventId::SendAdcRequest);
            sendEvent(eventSendAdcRequest, true);
            break;
        }
    case CommonEventId::SendAdcRequest:
        {
            mLog.importantInfo("%s: send ADC request", __FUNCTION__);
            gpio_set_level(GPIO_NUM_4, 1);
            vTaskDelay(5);
            adcVoltage voltage = mItsAdc.readAvrage(100);
            gpio_set_level(GPIO_NUM_4, 0);
            vTaskDelay(5);

            SEvent eventSendAdcRequest(CommonEventId::SendAdcRequest, sizeof(adcVoltage), 0, static_cast<void*>(&voltage));
            sendEvent(eventSendAdcRequest, false);
            break;
        }
    case CommonEventId::SendAdcACK:
        {
            mLog.importantInfo("%s: send ADC ACK", __FUNCTION__);
            break;
        }
    case CommonEventId::ReadBme280Request:
        {
            mLog.importantInfo("%s: read BME280 sensor data", __FUNCTION__);
            SEvent eventReadBme280ACK(CommonEventId::ReadBme280ACK);
            sendEvent(eventReadBme280ACK, false);
            SEvent eventSendBme280Request(CommonEventId::SendBme280Request);
            sendEvent(eventSendBme280Request, true);
            break;
        }
    case CommonEventId::SendBme280Request:
        {
            mLog.importantInfo("%s: read BME280 sensor data", __FUNCTION__);
            SBme280 bme280Data(0.0f, 0.0f, 0.0f);

            mItsBme280.readAvrage(bme280Data, 20);

            SEvent eventSendBme280Request(CommonEventId::SendBme280Request, sizeof(SBme280), 0, static_cast<void*>(&bme280Data));
            sendEvent(eventSendBme280Request, false);
            break;
        }
    case CommonEventId::SendBme280ACK:
        {
            mLog.importantInfo("%s: send BME280 ACK", __FUNCTION__);
            break;
        }
    default:
        {
            mLog.warning("%s: unknown event ID: %u", __FUNCTION__, static_cast<CommonEventId>(event.mEventId));
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
    CHardware& self = *static_cast<CHardware*>(pvParameter);

    while (1) 
    {
        processQueueEvent(self, CLoggerConfig::CLogModule::Hardware);
    }
}
