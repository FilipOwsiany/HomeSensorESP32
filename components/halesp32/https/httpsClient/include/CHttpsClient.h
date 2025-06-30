#ifndef __CHTTPSCLIENT_H__
#define __CHTTPSCLIENT_H__

#include "commonStd.h"

#include "IHttpsClient.h"

#include "esp_http_client.h"

class CHttpsClient : public IHttpsClient
{
private:
    SHttpClientRequest mRequest;

    bool validateRequest(SHttpClientRequest& request);
    void httpsStatusHandler(SHttpClientRequest& request);
    static esp_err_t httpsEventHandler(esp_http_client_event_t *event);
public:
    CHttpsClient();
    ~CHttpsClient();

    bool sendRequest(SHttpClientRequest& request) override;
};

#endif //__CHTTPSCLIENT_H__