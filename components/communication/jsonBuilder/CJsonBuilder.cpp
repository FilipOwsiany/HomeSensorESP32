#include "CJsonBuilder.h"

#include "CLogger.h"

CJsonBuilder::CJsonBuilder(/* args */)
{
    registerBuilder(EJsonBuilderType::Measurement, new CMeasurmentsBuilder());
    registerBuilder(EJsonBuilderType::Setting, new SettingsBuilder());
}

CJsonBuilder::~CJsonBuilder()
{
}

bool CJsonBuilder::buildHelper(void *input, cJSON *payloadObj, EJsonBuilderType type)
{
    printf("build_helper type %d\n", type);
    IJsonBuilder* builder = builders[type];
    if (builder) 
    {
        return builder->jsonBuild(input, payloadObj);
    }
    return false;
}

char* CJsonBuilder::buildMessage(void *input, EJsonBuilderType type)
{
    cJSON *message =    NULL;
    cJSON *payload =    NULL;
    char *json =        NULL;
    
    message = cJSON_CreateObject();
    CLEAN_UP_MESSAGE_IF_0(message);

    RET_IF_0(cJSON_AddNumberToObject(message, "Id", (double)1.0), NULL);
    CLEAN_UP_MESSAGE_IF_0(cJSON_AddStringToObject(message, "Name", "Sensor1"));

    payload = cJSON_CreateObject();
    CLEAN_UP_ALL_IF_0(payload);
    CLEAN_UP_ALL_IF_0(buildHelper(input, payload, type));
    CLEAN_UP_ALL_IF_0(cJSON_AddItemToObject(message, "SensorData", payload));

    json = cJSON_PrintUnformatted(message);
    cJSON_Delete(message);
    return json;
clean_up_all:
    cJSON_Delete(payload);
clean_up_message:
    cJSON_Delete(message);
    return NULL;
}

void CJsonBuilder::registerBuilder(EJsonBuilderType type, Json_builderBase* builder)
{
    if (type < EJsonBuilderType::Max)
    {
        builders[type] = builder;
    }
}

std::string CJsonBuilder::BuildJson(const SMeassurment& measurement);
{
    
    return  build_message(input, type);
}

CJsonBuilder& CJsonBuilder::getInstance()
{
    static CJsonBuilder instance;
    return instance;
}
