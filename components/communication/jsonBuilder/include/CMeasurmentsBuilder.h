#ifndef __CMEASURMENTBUILDER_H__
#define __CMEASURMENTBUILDER_H__

#include "IJsonBuilder.h"
#include "commonData.h"
#include "cJSON.h"

class CMeasurmentsBuilder : public IJsonBuilder
{
public:
    CMeasurmentsBuilder() = default;
    ~CMeasurmentsBuilder() = default;

    bool jsonBuild(void* input, cJSON* payloadObj) override
    {
        SMeassurment& measurement = *static_cast<SMeassurment*>(input);
        printf("CMeasurmentsBuilder::jsonBuild()\n");

        if (!cJSON_AddNumberToObject(payloadObj, "Value", measurement.mTemperature)) return false;
        if (!cJSON_AddNumberToObject(payloadObj, "Value2", measurement.mPressure)) return false;
        if (!cJSON_AddNumberToObject(payloadObj, "Value3", measurement.mHumidity)) return false;
        if (!cJSON_AddNumberToObject(payloadObj, "Value4", measurement.mVoltage)) return false;
        if (!cJSON_AddNumberToObject(payloadObj, "Timestamp", 1234)) return false;
        if (!cJSON_AddStringToObject(payloadObj, "SensorName", "TemperatureSensor")) return false;

        return true;
    }
};

#endif // __CMEASURMENTBUILDER_H__
