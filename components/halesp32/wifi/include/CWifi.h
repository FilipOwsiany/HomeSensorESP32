#ifndef __CWIFI_H__
#define __CWIFI_H__

#include "IWifi.h"

#include "commonStd.h"
#include "commonOs.h"

#include "esp_wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"


class CWifi : public IWifi
{
public:
    static CWifi& getInstance();

    CWifi(const CWifi&) = delete;
    CWifi& operator=(const CWifi&) = delete;

    void connect() override;
    void disconnect() override;
    bool isConnected() override;
    void setSSID(const std::string& ssid) override;
    void setPassword(const std::string& password) override;
    std::string getSSID() override;
    std::string getPassword() override;

private:

    EventGroupHandle_t wifiEventGroup = nullptr;
    uint32_t retryNum = 0;

    CWifi();
    ~CWifi();

    static void eventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);
};

#endif //__CWIFI_H__