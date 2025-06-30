#include "CJsonBuilder.h"
#include "CLogger.h"
#include "CMeasurmentsBuilder.h"
#include "CSettingsBuilder.h"

CJsonBuilder::CJsonBuilder()
{
    registerBuilder(EJsonBuilderType::Measurement, new CMeasurmentsBuilder());
    registerBuilder(EJsonBuilderType::Setting, new CSettingsBuilder());
}

CJsonBuilder::~CJsonBuilder()
{
    for (int i = 0; i < static_cast<int>(EJsonBuilderType::Max); i++)
    {
        delete builders[i];
        builders[i] = nullptr;
    }
}

void CJsonBuilder::registerBuilder(EJsonBuilderType type, IJsonBuilder* builder)
{
    if (type < EJsonBuilderType::Max)
    {
        builders[static_cast<int>(type)] = builder;
    }
}

bool CJsonBuilder::buildHelper(void* input, cJSON* payloadObj, EJsonBuilderType type)
{
    printf("CJsonBuilder::buildHelper(), type: %d\n", static_cast<int>(type));
    IJsonBuilder* builder = builders[static_cast<int>(type)];
    if (builder)
    {
        return builder->jsonBuild(input, payloadObj);
    }
    return false;
}

char* CJsonBuilder::buildMessage(void* input, EJsonBuilderType type)
{
    cJSON* message = cJSON_CreateObject();
    if (!message) return nullptr;

    if (!cJSON_AddNumberToObject(message, "Id", 1.0))
    {
        cJSON_Delete(message);
        return nullptr;
    }

    if (!cJSON_AddStringToObject(message, "Name", "Sensor1"))
    {
        cJSON_Delete(message);
        return nullptr;
    }

    cJSON* payload = cJSON_CreateObject();
    if (!payload)
    {
        cJSON_Delete(message);
        return nullptr;
    }

    if (!buildHelper(input, payload, type))
    {
        cJSON_Delete(payload);
        cJSON_Delete(message);
        return nullptr;
    }

    cJSON_AddItemToObject(message, "SensorData", payload);
    char* json = cJSON_PrintUnformatted(message);
    cJSON_Delete(message);
    return json;
}

char* CJsonBuilder::buildJson(EJsonBuilderType type, void* input)
{
    return buildMessage(input, type);
}

CJsonBuilder& CJsonBuilder::getInstance()
{
    static CJsonBuilder instance;
    return instance;
}
