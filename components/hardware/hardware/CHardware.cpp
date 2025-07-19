#include "CHardware.h"
#include "CHalEspressifFactory.h"
#include "driver/gpio.h"

#include "CBme280.h"

CHardware::CHardware(IBme280& aItsBme280, IAdc& aItsAdc, IStorage& aItsStorage):
    IEventListener(10), 
    CBaseTask("CHardware", 4096, 10, this, CHardware::taskFunction),
    mItsBme280(aItsBme280),
    mItsAdc(aItsAdc),
    mItsStorage(aItsStorage),
    mMeasQueue(10, aItsStorage),
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
    case CommonEventId::ReadMeasRequest:
        {
            mLog.importantInfo("%s: read measurement request", __FUNCTION__);
            SEvent eventReadMeasACK(CommonEventId::ReadMeasACK);
            sendEvent(eventReadMeasACK, false);

            SEvent eventSendMeasRequest(CommonEventId::SendMeasRequest);
            sendEvent(eventSendMeasRequest, true);
            break;
        }
    case CommonEventId::SendMeasRequest:
        {
            mLog.importantInfo("%s: send meas request", __FUNCTION__);
            mLog.debug("%s: reading ADC value", __FUNCTION__);
            gpio_set_level(GPIO_NUM_4, 1);
            vTaskDelay(5);
            adcVoltage voltage = mItsAdc.readAvrage(100);
            gpio_set_level(GPIO_NUM_4, 0);
            vTaskDelay(5);

            mLog.debug("%s: BME280 value read: %d", __FUNCTION__, voltage);
            SBme280 bme280Data(0.0f, 0.0f, 0.0f);

            mItsBme280.readAvrage(bme280Data, 4);

            mLog.debug("%s: prepare measurment", __FUNCTION__);
            SMeassurment meas(
                bme280Data.temperature, 
                bme280Data.humidity, 
                bme280Data.pressure, 
                static_cast<float>(voltage)
            );

            mLog.importantInfo("Measurement data - temperature: %.2f, humidity: %.2f, pressure: %.2f, voltage: %.2f",
                meas.mTemperature, meas.mHumidity, meas.mPressure, meas.mVoltage);
            mMeasQueue.addMeasurement(meas);

            if(mMeasQueue.isQueueFull()) 
            {
                mLog.importantInfo("%s: queue is full, sending data to server", __FUNCTION__);
                SMeassurmentMultiple measurements;
                if (mMeasQueue.readMeasurements(measurements)) 
                {
                    mLog.importantInfo("%s: measurements read successfully", __FUNCTION__);
                    SEvent eventSendDataToServer(CommonEventId::SendMeasRequest, sizeof(SMeassurmentMultiple), 0, static_cast<void*>(&measurements));
                    sendEvent(eventSendDataToServer, false);
                } 
                else 
                {
                    mLog.error("%s: failed to read measurements from queue", __FUNCTION__);
                }
            }
            else
            {
                mLog.importantInfo("%s: queue is not full, stopping application", __FUNCTION__);
                SEvent eventAplicationStop(CommonEventId::ApplicationStop);
                sendEvent(eventAplicationStop, false);
            }
            break;
        }
    case CommonEventId::SendMeasACK:
        {
            mLog.importantInfo("%s: send measurement ACK", __FUNCTION__);
            break;
        }
    // case CommonEventId::ReadBme280Request:
    //     {
    //         mLog.importantInfo("%s: read BME280 sensor data", __FUNCTION__);
    //         SEvent eventReadBme280ACK(CommonEventId::ReadBme280ACK);
    //         sendEvent(eventReadBme280ACK, false);
    //         SEvent eventSendBme280Request(CommonEventId::SendBme280Request);
    //         sendEvent(eventSendBme280Request, true);
    //         break;
    //     }
    // case CommonEventId::SendBme280Request:
    //     {
    //         mLog.importantInfo("%s: read BME280 sensor data", __FUNCTION__);
    //         SBme280 bme280Data(0.0f, 0.0f, 0.0f);

    //         mItsBme280.readAvrage(bme280Data, 20);
    //         SMeassurment meas(
    //             bme280Data.temperature, 
    //             bme280Data.humidity, 
    //             bme280Data.pressure, 
    //             0.0f // Voltage will be set later
    //         );
    //         mMeasQueue.addMeasurement(meas);

    //         if(mMeasQueue.isQueueFull()) 
    //         {
    //             mLog.importantInfo("%s: queue is full, sending data to server", __FUNCTION__);
    //             SMeassurmentMultiple measurements;
    //             if (mMeasQueue.readMeasurements(measurements)) 
    //             {
    //                 SEvent eventSendDataToServer(CommonEventId::SendDataToServerMultiple, sizeof(SMeassurmentMultiple), 0, static_cast<void*>(&measurements));
    //                 sendEvent(eventSendDataToServer, false);
    //             } 
    //             else 
    //             {
    //                 mLog.error("%s: failed to read measurements from queue", __FUNCTION__);
    //             }
    //         }
    //         else
    //         {
    //             SEvent eventAplicationStop(CommonEventId::ApplicationStop);
    //             sendEvent(eventAplicationStop, false);
    //         }
    //         break;
    //     }
    // case CommonEventId::SendBme280ACK:
    //     {
    //         mLog.importantInfo("%s: send BME280 ACK", __FUNCTION__);
    //         break;
    //     }
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
