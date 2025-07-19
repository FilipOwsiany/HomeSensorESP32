#ifndef __CMEASUREMENTMULTIPLEBUILDER_H__
#define __CMEASUREMENTMULTIPLEBUILDER_H__

#include "IJsonBuilder.h"
#include "commonData.h"
#include "cJSON.h"

class CMeasurementsMultipleBuilder : public IJsonBuilder
{
public:
    CMeasurementsMultipleBuilder() = default;
    ~CMeasurementsMultipleBuilder() = default;

    bool jsonBuild(void* input, cJSON* payloadObj) override
    {
        if (input == nullptr || payloadObj == nullptr)
            return false;

        SMeassurmentMultiple& measurements = *static_cast<SMeassurmentMultiple*>(input);

        // Create array to hold multiple measurement entries
        cJSON* measurementsArray = cJSON_CreateArray();
        if (measurementsArray == nullptr)
            return false;

        for (uint8_t i = 0; i < 10; ++i)
        {
            cJSON* measurementObj = cJSON_CreateObject();
            if (measurementObj == nullptr)
            {
                cJSON_Delete(measurementsArray);
                return false;
            }

            // Add each field with descriptive keys
            if (!cJSON_AddNumberToObject(measurementObj, "temperature", measurements.mTemperature[i]))
            {
                cJSON_Delete(measurementObj);
                cJSON_Delete(measurementsArray);
                return false;
            }
            if (!cJSON_AddNumberToObject(measurementObj, "pressure",    measurements.mPressure[i]))
            {
                cJSON_Delete(measurementObj);
                cJSON_Delete(measurementsArray);
                return false;
            }
            if (!cJSON_AddNumberToObject(measurementObj, "humidity",    measurements.mHumidity[i]))
            {
                cJSON_Delete(measurementObj);
                cJSON_Delete(measurementsArray);
                return false;
            }
            if (!cJSON_AddNumberToObject(measurementObj, "voltage",    measurements.mVoltage[i]))
            {
                cJSON_Delete(measurementObj);
                cJSON_Delete(measurementsArray);
                return false;
            }

            cJSON_AddItemToArray(measurementsArray, measurementObj);
        }

        // Attach the array under a single "measurements" key
        if (!cJSON_AddItemToObject(payloadObj, "measurements", measurementsArray))
        {
            cJSON_Delete(measurementsArray);
            return false;
        }

        return true;
    }
};

#endif // __CMEASUREMENTMULTIPLEBUILDER_H__
