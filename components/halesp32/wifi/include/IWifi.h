#ifndef __IWIFI_H__
#define __IWIFI_H__

#include "commonStd.h"

class IWifi
{
public:
    IWifi() = default;
    ~IWifi() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual void setSSID(const std::string& ssid) = 0;
    virtual void setPassword(const std::string& password) = 0;
    virtual std::string getSSID() = 0;
    virtual std::string getPassword() = 0;
};

#endif //__CWIFI_H__