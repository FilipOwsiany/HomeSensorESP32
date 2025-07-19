#include "CCommunication.h"
#include "CCommunicationConfig.h"

#include "CHttpsClient.h"

#include "SBme280.h"

#include "CLogger.h"

#include "CJsonBuilder.h"

#include "CommonData.h"

#define RESPONSE_BUF_SIZE 1024

static const HttpHeader _cloudHeaders[] = {
	{"Content-Type",	"application/json",	    },
};

CCommunication::CCommunication() : 
    IEventListener(10), 
    CBaseTask("CCommunication", 4096, 10, this, CCommunication::taskFunction),
    mItsHttpsClient(new CHttpsClient(*this)),
    mLog(CLoggerConfig::CLogModule::Communication)
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

CCommunication::~CCommunication()
{

}

void CCommunication::sendEvent(SEvent& event, bool selfNotify)
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

void CCommunication::onEvent(SEvent& event)
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

void CCommunication::responseCallback(SHttpClientRequest& event)
{
    printf("HTTPS response handler called with response code: %d\n", event.respCode);
    SEvent eventReadBme280Request(CommonEventId::ReadBme280Request);
    sendEvent(eventReadBme280Request, false);
}

void CCommunication::parseEvent(SEvent& event)
{
    mLog.importantInfo("%s: %d", __FUNCTION__, event.mEventId);
    switch (event.mEventId)
    {
    case CommonEventId::SendDataToServer:
        {
            mLog.debug("%s: sendDataToServer event received", __FUNCTION__);
            if (event.mData == nullptr)
            {
                mLog.error("%s: measurement data is null", __FUNCTION__);
                return;
            }
            SMeassurment measurement = *static_cast<SMeassurment*>(event.mData);
            mLog.importantInfo("Measurement data - temperature: %.2f, humidity: %.2f, pressure: %.2f, voltage: %.2f",
                               measurement.mTemperature, measurement.mHumidity, measurement.mPressure, measurement.mVoltage);

            static char repBuffer[RESPONSE_BUF_SIZE];
            memset(repBuffer, 0, RESPONSE_BUF_SIZE);

            char* postData = CJsonBuilder::getInstance().buildJson(
                CJsonBuilder::EJsonBuilderType::Measurement,
                &measurement
            );

            if (postData == nullptr)
            {
                mLog.error("Failed to build JSON data");
                return;
            }

            // Użycie postData np. do wysyłki HTTP POST...
            printf("%s\n", postData);

            SHttpClientRequest httpsRequest(
                const_cast<char*>(HOST_STRING),                     // Host
                443,                                                // Port
                const_cast<char*>(PATH_SINGLE_STRING),              // Path
                SHttpClientRequest::Method::Post,                   // Method
                true,                                               // Use HTTPS
                4000,                                               // Timeout in ms
                postData,                                           // Post data
                _cloudHeaders,                                      // Headers
                sizeof(_cloudHeaders) / sizeof(HttpHeader),         // Number of headers
                0,                                                  // Request ID (not used here)
                repBuffer,                                          // Response buffer
                RESPONSE_BUF_SIZE,                                  // Response buffer size
                nullptr,                                            // User data (not used here)
                0
            );
                        
            mItsHttpsClient->sendRequest(httpsRequest);
            printf("HTTPS request sent, result: %d\n", httpsRequest.result);
            if (httpsRequest.result == true) 
            {
                mLog.success("%s: HTTPS request sent successfully, response: %s", __FUNCTION__, repBuffer);
            } 
            else 
            {
                mLog.error("%s: HTTPS request failed with response code: %d", __FUNCTION__, httpsRequest.respCode);
            }

            SEvent eventSendMeasACK(CommonEventId::ApplicationStop);
            sendEvent(eventSendMeasACK, false);
            break;
        }
    case CommonEventId::SendDataToServerMultiple:
        {
            mLog.debug("%s: sendDataToServerMultiple event received", __FUNCTION__);
            if (event.mData == nullptr)
            {
                mLog.error("%s: measurement multiple data is null", __FUNCTION__);
                return;
            }
            SMeassurmentMultiple measurements = *static_cast<SMeassurmentMultiple*>(event.mData);
            for (int i = 0; i < 10; ++i) 
            {
                mLog.importantInfo("Measurement %d - temperature: %.2f, humidity: %.2f, pressure: %.2f, voltage: %.2f",
                                   i + 1, 
                                   measurements.mTemperature[i], 
                                   measurements.mHumidity[i], 
                                   measurements.mPressure[i], 
                                   measurements.mVoltage[i]);
            }

            static char repBuffer[RESPONSE_BUF_SIZE];
            memset(repBuffer, 0, RESPONSE_BUF_SIZE);

            char* postData = CJsonBuilder::getInstance().buildJson(
                CJsonBuilder::EJsonBuilderType::MeasurementMultiple,
                &measurements
            );

            if (postData == nullptr)
            {
                mLog.error("Failed to build JSON data");
                return;
            }

            // Użycie postData np. do wysyłki HTTP POST...
            printf("%s\n", postData);

            SHttpClientRequest httpsRequest(
                const_cast<char*>(HOST_STRING),                     // Host
                443,                                                // Port
                const_cast<char*>(PATH_BATCH_STRING),               // Pathath
                SHttpClientRequest::Method::Post,                   // Method
                true,                                               // Use HTTPS
                4000,                                               // Timeout in ms
                postData,                                           // Post data
                _cloudHeaders,                                      // Headers
                sizeof(_cloudHeaders) / sizeof(HttpHeader),         // Number of headers
                0,                                                  // Request ID (not used here)
                repBuffer,                                          // Response buffer
                RESPONSE_BUF_SIZE,                                  // Response buffer size
                nullptr,                                            // User data (not used here)
                0
            );
                        
            mItsHttpsClient->sendRequest(httpsRequest);
            printf("HTTPS request sent, result: %d\n", httpsRequest.result);
            if (httpsRequest.result == true) 
            {
                mLog.success("%s: HTTPS request sent successfully, response: %s", __FUNCTION__, repBuffer);
            } 
            else 
            {
                mLog.error("%s: HTTPS request failed with response code: %d", __FUNCTION__, httpsRequest.respCode);
            }

            SEvent eventSendMeasACK(CommonEventId::ApplicationStop);
            sendEvent(eventSendMeasACK, false);
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

void CCommunication::taskFunction(void* pvParameter)
{
    CCommunication& self = *static_cast<CCommunication*>(pvParameter);

    while (1) 
    {
        processQueueEvent(self, CLoggerModule::Communication);
    }
}
