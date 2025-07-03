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

    SHttpClientRequest();
    SHttpClientRequest(char* aHost, int aPort, char* aPath, Method aMethod,
                                              bool aUseHttps, int aTimeout, char* aPostData,
                                              const HttpHeader* aHeaders, uint8_t aHeadersNum,
                                              uint8_t aReqId, char* aRxBuffer, uint16_t aRxBufferSize,
                                              void* aUserData, uint16_t aRespOffset);
                                            
    SHttpClientRequest& operator=(const SHttpClientRequest& other);
};

inline SHttpClientRequest::SHttpClientRequest()
    : host(nullptr),
      port(80),
      path(nullptr),
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
      respCode(static_cast<int>(Error::FailedToConnect))
{}

inline SHttpClientRequest::SHttpClientRequest(char* aHost, int aPort, char* aPath, Method aMethod,
                                              bool aUseHttps, int aTimeout, char* aPostData,
                                              const HttpHeader* aHeaders, uint8_t aHeadersNum,
                                              uint8_t aReqId, char* aRxBuffer, uint16_t aRxBufferSize,
                                              void* aUserData, uint16_t aRespOffset)
    : host(aHost),
      port(aPort),
      path(aPath),
      method(aMethod),
      useHttps(aUseHttps),
      timeout(aTimeout),
      postData(aPostData),
      headers(aHeaders),
      headersNum(aHeadersNum),
      reqId(aReqId),
      rxBuffer(aRxBuffer),
      rxBufferSize(aRxBufferSize),
      userData(aUserData),
      respOffset(aRespOffset),
      result(false),
      respCode(0)
{}

inline SHttpClientRequest& SHttpClientRequest::operator=(const SHttpClientRequest& other)
{
    if (this != &other)
    {
        host = other.host;
        port = other.port;
        path = other.path;
        method = other.method;
        useHttps = other.useHttps;
        timeout = other.timeout;
        postData = other.postData;
        headers = other.headers;
        headersNum = other.headersNum;
        reqId = other.reqId;
        rxBuffer = other.rxBuffer;
        rxBufferSize = other.rxBufferSize;
        userData = other.userData;
        respOffset = other.respOffset;
        result = other.result;
        respCode = other.respCode;
    }
    return *this;
}


#endif //__SHTTPSCLIENTREQUEST_H__
