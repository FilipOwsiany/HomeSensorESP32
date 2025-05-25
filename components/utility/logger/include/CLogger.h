#ifndef __CLOGGER_H__
#define __CLOGGER_H__

#include "commonStd.h"
#include "commonOs.h"

#include "CLoggerConfig.h"

using CLoggerModule = CLoggerConfig::CLogModule;
using CLoggerLevel  = CLoggerConfig::CLogLevel;

class CLogger : public CLoggerConfig
{
public:
    static void log(CLoggerConfig::CLogModule module, CLoggerConfig::CLogLevel logLvl, const char* format, ...);
private:
    CLogger(CLogger* instance) = delete;
    CLogger(CLogger& instance) = delete;
    void operator=(const CLogger &) = delete;
    ~CLogger() = delete;
};


#endif //__CLOGGER_H__