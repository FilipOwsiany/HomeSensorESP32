#include "CHttpsClient.h"

#include "CLogger.h"

CHttpsClient::CHttpsClient(IHttpsClientCallback& aCallback): 
	mRequest(),
	mCallback(aCallback)
{

}

CHttpsClient::~CHttpsClient()
{

}

bool CHttpsClient::validateRequest(SHttpClientRequest& request)
{
	if (!request.host)
	{
        CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "host is NULL");
		request.respCode = static_cast<int>(SHttpClientRequest::Error::BadConfig);
		return false;
	}

	if (!request.path)
	{
        CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "path is NULL");
		request.respCode = static_cast<int>(SHttpClientRequest::Error::BadConfig);
		return false;
	}

	if (!request.rxBuffer && request.rxBufferSize > 0)
	{
        CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "rxBufferSize > 0 but rxBuffer is NULL");
		request.respCode = static_cast<int>(SHttpClientRequest::Error::BadConfig);
		return false;
	}

	return true;
}

void CHttpsClient::httpsStatusHandler(SHttpClientRequest& request)
{
	char *errMsg;
	switch (request.respCode)
	{
	case 200:
	case 201:
	case 204:
		errMsg = "OK";
		request.result = true;
		CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Success, "http request of type %d returned %s code: %d, result %d\n", request.reqId, errMsg, request.respCode, request.result);
		return;
	case 401:
	case 404:
        request.result = false;	
		errMsg = "CLIENT ERROR";
		CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "http request of type %d returned %s code: %d, result %d", request.reqId, errMsg, request.respCode, request.result);
		break;
	case 500:
	    request.result = false;
		errMsg = "SERVER ERROR";
		CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "http request of type %d returned %s code: %d, result %d", request.reqId, errMsg, request.respCode, request.result);
		break;
	default:
	    request.result = false;
		errMsg = "unhandled error";
    	CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "http request of type %d returned %s code: %d, result %d", request.reqId, errMsg, request.respCode, request.result);
		break;
	}
}

esp_err_t CHttpsClient::httpsEventHandler(esp_http_client_event_t *event)
{
    CHttpsClient *self = static_cast<CHttpsClient *>(event->user_data);

    CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "httpsEventHandler: %d", event->event_id);

	switch (event->event_id)
	{
	case HTTP_EVENT_ON_CONNECTED:
		if (self->mRequest.rxBuffer)
		{
			memset(self->mRequest.rxBuffer, 0, self->mRequest.rxBufferSize);
		}
		self->mRequest.respOffset = 0;
		break;
	case HTTP_EVENT_ON_DATA:
		if (self->mRequest.rxBuffer)
		{
			if (self->mRequest.respOffset + event->data_len <= self->mRequest.rxBufferSize)
			{
				memcpy(self->mRequest.rxBuffer + self->mRequest.respOffset, event->data, event->data_len);
				self->mRequest.respOffset += event->data_len;
				CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "Received %d bytes of data", event->data_len);
				CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "Current response buffer: %.*s", self->mRequest.respOffset, self->mRequest.rxBuffer);
			}
			else
			{
                CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "Insufficient buffer size to read http response");
			}
		}
		break;
	case HTTP_EVENT_ON_FINISH:
		CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "HTTP_EVENT_ON_FINISH");
		// TODO if !use_https AES decode
		self->mRequest.respCode = esp_http_client_get_status_code(event->client);
		self->httpsStatusHandler(self->mRequest);
		self->mCallback.responseCallback(self->mRequest);
		break;
	case HTTP_EVENT_ERROR:
	case HTTP_EVENT_HEADERS_SENT:
	case HTTP_EVENT_ON_HEADER:
	case HTTP_EVENT_DISCONNECTED:
	case HTTP_EVENT_REDIRECT:
		break;
	default:
        CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "Unknown event id: %d", event->event_id);
		break;
	}
	return ESP_OK;
}

bool CHttpsClient::sendRequest(SHttpClientRequest& request)
{
    mRequest = request;

	char *postData = mRequest.postData;
	mRequest.result = false;
	mRequest.respCode = static_cast<int>(SHttpClientRequest::Error::FailedToConnect);

	if (!this->validateRequest(request))
	{
        delete[] postData;
		return false;
	}

    esp_http_client_config_t config = {};
    config.host = mRequest.host;
    config.port = mRequest.port;
    config.path = mRequest.path;
    config.event_handler = httpsEventHandler;
    config.transport_type = mRequest.useHttps ? HTTP_TRANSPORT_OVER_SSL : HTTP_TRANSPORT_OVER_TCP;
    config.timeout_ms = mRequest.timeout;
    config.user_data = static_cast<void*>(this);
    config.buffer_size_tx = 1536;

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, (esp_http_client_method_t)mRequest.method);

    for (uint8_t i = 0; i < mRequest.headersNum; i++)
	{
		if (mRequest.headers[i].key && mRequest.headers[i].value)
		{
			esp_http_client_set_header(client, mRequest.headers[i].key, mRequest.headers[i].value);
		}
	}

    if (mRequest.method == SHttpClientRequest::Method::Post)
    {
        if (!postData)
        {
            CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Error, "Post data is NULL");
            esp_http_client_cleanup(client);

			request = mRequest;
			return false;
        }
		// TODO if !useHttps AES encode
        esp_http_client_set_post_field(client, postData, strlen(postData));
    }

    esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK)
	{
		CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "HTTPS Status = %d, content_length = %lld",
			esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
	}
	else if (err == ESP_ERR_HTTP_EAGAIN)
	{
        CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "Cloud request send timeout");
	}
	else
	{
        CLogger::log(CLoggerModule::HttpsClient, CLoggerLevel::Debug, "Error perform http request %s", esp_err_to_name(err));
	}

	esp_http_client_cleanup(client);

	request = mRequest;
    return true;
}
