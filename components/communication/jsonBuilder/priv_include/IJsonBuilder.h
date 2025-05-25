#ifndef __IJSONBUILDER_H__
#define __IJSONBUILDER_H__

#include "commonStd.h"
#include "cJSON.h"

class IJsonBuilder
{
public:

    virtual ~IJsonBuilder() = 0;
    virtual bool jsonBuild(void *input, cJSON *payloadObj) = 0;
};

IJsonBuilder::~IJsonBuilder() {}

#endif //__IJSONBUILDER_H__