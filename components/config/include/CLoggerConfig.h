#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__

#include "commonStd.h"

#define LOG_ENABLE  0

class CLoggerConfig
{
private:
    /* data */
public:

    enum class CLogLevel
    {
        None,
        CriticalFailure,
        Error,
        Warning,
        Success,
        Performance,
        ImportantInfo,
        Info,
        Debug,
    };

    enum class CLogModule
    {
        Main,
        EventPublisherManager,
        BaseQueue,
        BaseSemaphore,
        BaseTask,
        MessageQueue,
        Hardware,
        Control,
        Communication,
        Wifi,
        HttpsClient,
        Adc,
        Bme280,

        All,

        Num,
    };

    struct SConfig
    {
        CLogLevel logLevel;
        CLogModule module;
        bool isLogEnable;
        std::string prefix;
    };

    static const uint8_t lm_num = static_cast<uint8_t>(CLogModule::Num);

    static SConfig logConfigTable[lm_num];
};

#endif //__COMMON_LOGGER_H__