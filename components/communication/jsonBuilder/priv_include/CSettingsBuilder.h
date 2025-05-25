#ifndef __CSETTINGSBUILDER_H__
#define __CSETTINGSBUILDER_H__

#include "IJsonBuilder.h"
#include "commonData.h"
#include "cJSON.h"

class SettingsBuilder : public IJsonBuilder
{
public:
    SettingsBuilder() {};
    ~SettingsBuilder() {};
    virtual bool jsonBuild(void *input, cJSON *payloadObj) override
    {
        SSettings& settings = static_cast<SSettings&>(input);

        if(CJSON_AddNumberToObject(payloadObj, "wakeupTimeout", (double)settings.mWakeupTimeout) == NULL)
        {
            return false;
        }
        if(CJSON_AddNumberToObject(payloadObj, "sleepTimeout", (double)settings.mSleepTimeout) == NULL)
        {
            return false;
        }
        if(CJSON_AddNumberToObject(payloadObj, "measurementInterval", (double)settings.mMeasurementInterval) == NULL)
        {
            return false;
        }
        return true;
    }
};

#endif //__SETTINGS_BUILDER_HPP__
