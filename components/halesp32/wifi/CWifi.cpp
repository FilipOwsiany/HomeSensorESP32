#include "CWifi.h"

#include "CLogger.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void CWifi::eventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
    CWifi* self = static_cast<CWifi*>(arg);

    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
    } 
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED) 
    {
        if (self->retryNum < 10) 
        {
            esp_wifi_connect();
            self->retryNum++;
            CLogger::log(CLoggerModule::Control, CLoggerLevel::Warning, "Retry to connect to the AP");
        } 
        else 
        {
            xEventGroupSetBits(self->wifiEventGroup, WIFI_FAIL_BIT);
        }
        CLogger::log(CLoggerModule::Control, CLoggerLevel::Error, "connect to the AP fail");
    } 
    else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) eventData;
        CLogger::log(CLoggerModule::Control, CLoggerLevel::Success, "Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        self->retryNum = 0;
        xEventGroupSetBits(self->wifiEventGroup, WIFI_CONNECTED_BIT);
    }
}

CWifi::CWifi()
{
    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::Debug, "CWifi::CWifi()");
    wifiEventGroup = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instanceAnyId;
    esp_event_handler_instance_t instanceGotIp;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &eventHandler,
                                                        static_cast<void*>(this),
                                                        &instanceAnyId));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &eventHandler,
                                                        static_cast<void*>(this),
                                                        &instanceGotIp));
}

CWifi::~CWifi()
{
    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::Debug, "CWifi::~CWifi()");
}

CWifi& CWifi::getInstance()
{
    static CWifi instance;
    return instance;
}

void CWifi::connect()
{
    wifi_config_t wifiConfig = {};
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    
    strncpy((char *)wifiConfig.sta.ssid, "NETIASPOT-2.4GHz-xu8M", sizeof(wifiConfig.sta.ssid) - 1);
    strncpy((char *)wifiConfig.sta.password, "7JqvBQjq", sizeof(wifiConfig.sta.password) - 1);
    
    wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifiConfig.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;

    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::ImportantInfo, "Wifi config:");
    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::ImportantInfo, "SSID: %s", wifiConfig.sta.ssid);
    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::ImportantInfo, "Password: %s", wifiConfig.sta.password);
    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::ImportantInfo, "Auth mode: %d", wifiConfig.sta.threshold.authmode);
    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::ImportantInfo, "SAE PWE: %d", wifiConfig.sta.sae_pwe_h2e);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

    CLogger::log(CLoggerModule::Wifi, CLoggerLevel::Debug, "Wifi connecting...");

    EventBits_t bits = xEventGroupWaitBits(wifiEventGroup,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) 
    {
        CLogger::log(CLoggerModule::Wifi, CLoggerLevel::Success, "connected to ap SSID:%s password:%s",
                 "NETIASPOT-2.4GHz-xu8M", "7JqvBQjq");
    } 
    else if (bits & WIFI_FAIL_BIT) 
    {
        CLogger::log(CLoggerModule::Wifi, CLoggerLevel::Error, "Failed to connect to SSID:%s, password:%s",
                 "NETIASPOT-2.4GHz-xu8M", "7JqvBQjq");
    } 
    else 
    {
        CLogger::log(CLoggerModule::Wifi, CLoggerLevel::Error, "UNEXPECTED EVENT");
    }
}

void CWifi::disconnect()
{

}

bool CWifi::isConnected()
{
    return false;
}

void CWifi::setSSID(const std::string& ssid)
{

}

void CWifi::setPassword(const std::string& password)
{

}

std::string CWifi::getSSID()
{
    return "";
}

std::string CWifi::getPassword()
{
    return "";
}


