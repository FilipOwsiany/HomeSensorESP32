#include "CLoggerHelper.h"

#define LOGGER_HELPER_LOG(level) \
    va_list args; \
    va_start(args, format); \
    CLogger::log(mModule, CLoggerConfig::CLogLevel::level, format, args); \
    va_end(args);


CLoggerHelper::CLoggerHelper(CLoggerConfig::CLogModule aModule) :
    mModule(aModule)
{

}

void CLoggerHelper::criticalFailure(const char* format, ...)
{
    LOGGER_HELPER_LOG(CriticalFailure);
}

void CLoggerHelper::error(const char* format, ...)
{
    LOGGER_HELPER_LOG(Error);
}

void CLoggerHelper::warning(const char* format, ...)
{
    LOGGER_HELPER_LOG(Warning);
}

void CLoggerHelper::success(const char* format, ...)
{
    LOGGER_HELPER_LOG(Success);
}

void CLoggerHelper::performance(const char* format, ...)
{
    LOGGER_HELPER_LOG(Performance);
}

void CLoggerHelper::importantInfo(const char* format, ...)
{
    LOGGER_HELPER_LOG(ImportantInfo);
}

void CLoggerHelper::info(const char* format, ...)
{
    LOGGER_HELPER_LOG(Info);
}

void CLoggerHelper::debug(const char* format, ...)
{
    LOGGER_HELPER_LOG(Debug)
}
