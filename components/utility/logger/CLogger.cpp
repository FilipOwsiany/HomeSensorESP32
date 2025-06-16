
#include "CLogger.h"

#include "esp_log.h"

void CLogger::log(CLoggerConfig::CLogModule module, CLoggerConfig::CLogLevel logLvl, const char* format, ...)
{
    const auto& entry = CLoggerConfig::logConfigTable[static_cast<uint8_t>(module)];

    return;

    if (!entry.isLogEnable) 
    {
        return;
    }
    if (entry.logLevel < logLvl) 
    {
        return;
    }

    std::string logString;

    switch (logLvl)
    {
        case CLoggerConfig::CLogLevel::Debug:                                           break;
        case CLoggerConfig::CLogLevel::Info:                                            break;
        case CLoggerConfig::CLogLevel::ImportantInfo:   logString += "\033[0;34m";      break;
        case CLoggerConfig::CLogLevel::Success:         logString += "\033[0;36m";      break;
        case CLoggerConfig::CLogLevel::Performance:                                     break;
        case CLoggerConfig::CLogLevel::Warning:         logString += "\033[0;33m";      break;
        case CLoggerConfig::CLogLevel::Error:           logString += "\033[4;31m";      break;
        case CLoggerConfig::CLogLevel::CriticalFailure: logString += "\033[4;31m";      break;
        default: break;
    }

    logString += "[" + std::to_string(esp_log_timestamp()) + "] ";

    switch (logLvl)
    {
        case CLoggerConfig::CLogLevel::Debug:           logString += "[DEBUG] ";                break;
        case CLoggerConfig::CLogLevel::Info:            logString += "[INFO] ";                 break;
        case CLoggerConfig::CLogLevel::ImportantInfo:   logString += "\033[0;34m[IMPORTANT] ";  break;
        case CLoggerConfig::CLogLevel::Success:         logString += "\033[0;36m[SUCCESS] ";    break;
        case CLoggerConfig::CLogLevel::Performance:     logString += "[PERFORMANCE] ";          break;
        case CLoggerConfig::CLogLevel::Warning:         logString += "\033[0;33m[WARNING] ";    break;
        case CLoggerConfig::CLogLevel::Error:           logString += "\033[4;31m[ERROR] ";      break;
        case CLoggerConfig::CLogLevel::CriticalFailure: logString += "\033[4;31m[CRITICAL] ";   break;
        default: break;
    }

    logString += entry.prefix + ": ";

    if (logString.length() > 128)
    {
        return;
    }

    char messageBuffer[512];

    va_list args;
    va_start(args, format);
    vsnprintf(messageBuffer, sizeof(messageBuffer), format, args);
    va_end(args);

    logString += messageBuffer;

    logString += "\033[0m\n";

    printf("%s", logString.c_str());
}