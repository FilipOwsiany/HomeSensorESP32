#ifndef __CJSONBUILDER_H__
#define __CJSONBUILDER_H__

#include "commonStd.h"
#include "commonData.h"
#include "IJsonBuilder.h"
#include "cJSON.h"

class CJsonBuilder
{
public:
    enum class EJsonBuilderType
    {
        Measurement,
        Setting,
        Max
    };

    static CJsonBuilder& getInstance();

    CJsonBuilder(const CJsonBuilder&) = delete;
    CJsonBuilder& operator=(const CJsonBuilder&) = delete;

    char* buildJson(EJsonBuilderType type, void* input);

private:
    IJsonBuilder* builders[static_cast<int>(EJsonBuilderType::Max)] = { nullptr };

    CJsonBuilder();
    ~CJsonBuilder();

    bool buildHelper(void* input, cJSON* payloadObj, EJsonBuilderType type);
    char* buildMessage(void* input, EJsonBuilderType type);
    void registerBuilder(EJsonBuilderType type, IJsonBuilder* builder);
};

#endif // __CJSONBUILDER_H__
