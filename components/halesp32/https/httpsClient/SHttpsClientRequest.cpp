#include "SHttpsClientRequest.h"

SHttpsClientRequest::SHttpsClientRequest()
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
  callback(defaultHttpClientCallback),
  respOffset(0),
  result(false),
  respCode(0)
{}

void SHttpsClientRequest::defaultHttpClientCallback(void* userData, const char* response) 
{

}