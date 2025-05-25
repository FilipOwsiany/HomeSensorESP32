#ifndef __SHTTPSCLIENTREQUEST_H__
#define __SHTTPSCLIENTREQUEST_H__

#include "commonStd.h"

struct HttpHeader 
{
    char* key;
    char* value;
};

struct SHttpClientRequest 
{
    enum class Error 
    {
        FailedToConnect,
        BadConfig,
        Busy,
        Count
    };

    enum class Method 
    {
        Get    = 0,
        Post   = 1,
        Delete = 4
    };

    char* host;
    int port;
    char* path;
    Method method;
    bool useHttps;
    int timeout;
    char* postData;
    const HttpHeader* headers;
    uint8_t headersNum;
    uint8_t reqId;
    char* rxBuffer;
    uint16_t rxBufferSize;
    void* userData;
    uint16_t respOffset;
    bool result;
    int respCode;

    // callback pointer (declared after typedef!)
    void (*callback)(SHttpClientRequest&);

    SHttpClientRequest();
};

// ✅ teraz możemy użyć callbacka dopiero po pełnej definicji SHttpClientRequest
using HttpCallback = void(*)(SHttpClientRequest&);

// ✅ domyślny callback
inline void defaultHttpClientCallback(SHttpClientRequest& request) 
{
    // no-op
}

// ✅ konstruktor (może być też w .cpp jeśli wolisz)
inline SHttpClientRequest::SHttpClientRequest()
    : host(const_cast<char*>("")),
      port(80),
      path(const_cast<char*>("")),
      method(Method::Get),
      useHttps(false),
      timeout(5000),
      postData(nullptr),
      headers(nullptr),
      headersNum(0),
      reqId(0),
      rxBuffer(nullptr),
      rxBufferSize(0),
      userData(nullptr),
      respOffset(0),
      result(false),
      respCode(0),
      callback(defaultHttpClientCallback)
{}

#endif //__SHTTPSCLIENTREQUEST_H__
