#ifndef __IHTTPSCLIENTCALLBACK_H__
#define __IHTTPSCLIENTCALLBACK_H__

#include "commonStd.h"

#include "SHttpsClientRequest.h"

class IHttpsClientCallback
{
public:
    virtual ~IHttpsClientCallback() = default;

    virtual void responseCallback(SHttpClientRequest& event) = 0;
};

#endif //__IHTTPSCLIENTCALLBACK_H__