#include "CControl.h"

#include "CHalEspressifFactory.h"

#include "CWifi.h"
#include "CHttpsClient.h"

// #define RESPONSE_BUF_SIZE 1024

// static const HttpHeader _cloudHeaders[] = {
// 	{"Content-Type",	"application/json",	    },
// };

CControl::CControl(CLowPower& aItsLowPower) : 
    IEventListener(10), 
    CBaseTask("CControl", 4096, 10, this, CControl::taskFunction), 
    mItsWifi(CHalEspressifFactory::createWifi()),
    mItsLowPower(aItsLowPower),
    mLog(CLoggerConfig::CLogModule::Control)
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

}

CControl::~CControl()
{

}

void CControl::sendEvent(SEvent& event, bool selfNotify)
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

void CControl::onEvent(SEvent& event)
{
    printf("CControl::onEvent() event ID: %d\n\n\n", static_cast<int>(event.mEventId));
    mLog.debug("%s", __FUNCTION__);
    if (event.mEventId == CommonEventId::None) 
    {
        mLog.error("%s: invalid event data", __FUNCTION__);
        return;
    }
    queueSend(event, portMAX_DELAY);
    taskResume();
}

void CControl::parseEvent(SEvent& event)
{
    mLog.importantInfo("%s: %d", __FUNCTION__, event.mEventId);
    switch (event.mEventId)
    {
    case CommonEventId::ApplicationStart:
        {
            mLog.importantInfo("%s: application start event received", __FUNCTION__);
            SEvent eventReadMeasRequest(CommonEventId::ReadMeasRequest);
            sendEvent(eventReadMeasRequest, false);
            break;
        }
    case CommonEventId::ReadMeasACK:
        {
            mLog.importantInfo("%s: read meas value ACK", __FUNCTION__);
            break;
        }
    case CommonEventId::SendMeasRequest:
        {
            mLog.importantInfo("%s: send meas request", __FUNCTION__);
            SMeassurmentMultiple measurements = *(static_cast<SMeassurmentMultiple*>(event.mData));
            for (int i = 0; i < 10; ++i) 
            {
                mLog.importantInfo("Measurement %d - temperature: %.2f, humidity: %.2f, pressure: %.2f, voltage: %.2f",
                                   i + 1, measurements.mTemperature[i], measurements.mHumidity[i], 
                                   measurements.mPressure[i], measurements.mVoltage[i]);
            }

            mItsWifi.connect();
            mLog.success("%s: wifi connected", __FUNCTION__);

            SEvent eventSendMeasACK(CommonEventId::SendMeasACK);
            sendEvent(eventSendMeasACK, false);

            SEvent eventSendDataToServerMultiple(CommonEventId::SendDataToServerMultiple, sizeof(SMeassurmentMultiple), 0, static_cast<void*>(&measurements));
            sendEvent(eventSendDataToServerMultiple, false);
            break;
        }
    // case CommonEventId::ReadBme280ACK:
    //     {
    //         mLog.importantInfo("%s: read BME280 sensor data ACK", __FUNCTION__);
    //         break;
    //     }
    // case CommonEventId::SendBme280Request:
    //     {
    //         mLog.importantInfo("%s: send BME280 request with ADC value", __FUNCTION__);
    //         if (event.mData == nullptr)
    //         {
    //             mLog.error("%s: BME280 data is null", __FUNCTION__);
    //             break;
    //         }
    //         SBme280 bme280Data = *static_cast<SBme280*>(event.mData);

    //         mLog.importantInfo("BME280 data - temperature: %.2f, pressure: %.2f, humidity: %.2f",
    //                            bme280Data.temperature, bme280Data.pressure, bme280Data.humidity);

    //         mMeasurement.mTemperature = bme280Data.temperature;
    //         mMeasurement.mHumidity = bme280Data.humidity;
    //         mMeasurement.mPressure = bme280Data.pressure;

    //         mItsWifi.connect();
    //         mLog.success("%s: wifi connected", __FUNCTION__);

    //         SEvent eventSendBme280ACK(CommonEventId::SendBme280ACK);
    //         sendEvent(eventSendBme280ACK, false);

    //         SEvent eventSendDataToServer(CommonEventId::SendDataToServer, sizeof(SMeassurment), 0, static_cast<void*>(&mMeasurement));
    //         sendEvent(eventSendDataToServer, false);

    //         SEvent eventDummy(CommonEventId::dummy);
    //         sendEvent(eventDummy, true);
    //         break;
    //     }
    // case CommonEventId::dummy:
    //     {
    //         mLog.importantInfo("%s: dummy event received", __FUNCTION__);
    //         // This is a placeholder for future events
    //         SMeassurmentMultiple multipleData;
    //         for (int i = 0; i < 10; ++i) 
    //         {
    //             multipleData.mTemperature[i] = 35 + i;
    //             multipleData.mHumidity[i] = 20 + i;
    //             multipleData.mPressure[i] = 99999 + i;
    //             multipleData.mVoltage[i] = 3.3 + i;
    //         }

    //         SEvent eventSendDataToServerMultiple(CommonEventId::SendDataToServerMultiple, sizeof(SMeassurmentMultiple), 0, static_cast<void*>(&multipleData));
    //         sendEvent(eventSendDataToServerMultiple, false);
    //         break;
    //     }
    case CommonEventId::ApplicationStop:
        {
            mLog.success("%s: application stop event received", __FUNCTION__);
            mItsLowPower.enterDeepSleep(60 * 1000000);
            mLog.success("%s: device is going to deep sleep", __FUNCTION__);
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

void CControl::taskFunction(void* pvParameter)
{
    CControl& self = *static_cast<CControl*>(pvParameter);

    while (1) 
    {
        processQueueEvent(self, CLoggerModule::Control);
    }
}
