#ifndef __CMEASURMENTBUILDER_H__
#define __CMEASURMENTBUILDER_H__

#include "IJsonBuilder.h"
#include "commonData.h"
#include "cJSON.h"

class CMeasurmentsBuilder : public IJsonBuilder
{
public:
    CMeasurmentsBuilder() {};
    ~CMeasurmentsBuilder() {};
    virtual bool jsonBuild(void *input, cJSON *payloadObj) override
    {
        SMeassurment& measurment = static_cast<SMeassurment&>(input);
        printf("MeasurmentsBuilder::Json_build\n");
        if(cJSON_AddNumberToObject(payloadObj, "Value", (double)measurment.mTemperature) == NULL)
        {
            return false;
        }
        if(cJSON_AddNumberToObject(payloadObj, "Value1", (double)measurment.mHumidity) == NULL)
        {
            return false;
        }
        if(cJSON_AddNumberToObject(payloadObj, "Value2", (double)measurment.mPressure) == NULL)
        {
            return false;
        }
        if (cJSON_AddNumberToObject(payloadObj, "Timestamp", (double)1234) == NULL)
        {
            return false;
        }
        if (cJSON_AddStringToObject(payloadObj, "SensorName", "TemperatureSensor") == NULL)
        {
            return false;
        }
        return true;
    }
};

#endif //__MEASURMENTBUILDER_H__
