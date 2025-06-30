#ifndef __CLOGGERHELPER_H__
#define __CLOGGERHELPER_H__

#include "CLogger.h"

class CLoggerHelper
{
private:
    CLoggerConfig::CLogModule mModule;

public:
    CLoggerHelper(CLoggerConfig::CLogModule aModule)
        : mModule(aModule) {}

    template<typename... Args>
    void criticalFailure(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::CriticalFailure, format, args...);
    }

    template<typename... Args>
    void error(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::Error, format, args...);
    }

    template<typename... Args>
    void warning(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::Warning, format, args...);
    }

    template<typename... Args>
    void success(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::Success, format, args...);
    }

    template<typename... Args>
    void performance(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::Performance, format, args...);
    }

    template<typename... Args>
    void importantInfo(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::ImportantInfo, format, args...);
    }

    template<typename... Args>
    void info(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::Info, format, args...);
    }

    template<typename... Args>
    void debug(const char* format, Args... args)
    {
        CLogger::log(mModule, CLoggerConfig::CLogLevel::Debug, format, args...);
    }
};

#endif // __CLOGGERHELPER_H__
