#ifndef __IHTTPSCLIENT_H__
#define __IHTTPSCLIENT_H__

#include "commonStd.h"

#include "SHttpsClientRequest.h"


class IHttpsClient
{
public:
    virtual ~IHttpsClient() = default;

    virtual bool sendRequest(SHttpClientRequest& request) = 0;
};

#endif //__IHTTPSCLIENT_H__