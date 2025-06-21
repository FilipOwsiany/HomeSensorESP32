#ifndef __CLOGGERHELPER_H__
#define __CLOGGERHELPER_H__

#include "CLogger.h"
#include <cstdarg>

class CLoggerHelper
{
private:
    CLoggerConfig::CLogModule mModule;
public:
    CLoggerHelper(CLoggerConfig::CLogModule aModule);
    ~CLoggerHelper() = default;

    void criticalFailure(const char* format, ...);
    void error(const char* format, ...);
    void warning(const char* format, ...);
    void success(const char* format, ...);
    void performance(const char* format, ...);
    void importantInfo(const char* format, ...);
    void info(const char* format, ...);
    void debug(const char* format, ...);
};

#endif // __CLOGGERHELPER_H__