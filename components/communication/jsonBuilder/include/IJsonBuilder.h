#ifndef __IJSONBUILDER_H__
#define __IJSONBUILDER_H__

#include "commonStd.h"
#include "cJSON.h"

class IJsonBuilder
{
public:

    virtual ~IJsonBuilder() = default;
    virtual bool jsonBuild(void *input, cJSON *payloadObj) = 0;
};

#endif //__IJSONBUILDER_H__