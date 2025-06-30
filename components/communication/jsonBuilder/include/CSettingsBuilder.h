#ifndef __CSETTINGSBUILDER_H__
#define __CSETTINGSBUILDER_H__

#include "IJsonBuilder.h"
#include "commonData.h"
#include "cJSON.h"

class CSettingsBuilder : public IJsonBuilder
{
public:
    CSettingsBuilder() = default;
    ~CSettingsBuilder() = default;

    bool jsonBuild(void* input, cJSON* payloadObj) override
    {
        SSettings& settings = *static_cast<SSettings*>(input);
        if (!cJSON_AddNumberToObject(payloadObj, "wakeupTimeout", settings.mWakeupTimeout)) return false;
        if (!cJSON_AddNumberToObject(payloadObj, "sleepTimeout", settings.mSleepTimeout)) return false;
        if (!cJSON_AddNumberToObject(payloadObj, "measurementInterval", settings.mMeasurementInterval)) return false;
        return true;
    }
};

#endif // __CSETTINGSBUILDER_H__
