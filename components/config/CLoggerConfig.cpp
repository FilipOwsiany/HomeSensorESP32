#include "CLoggerConfig.h"

CLoggerConfig::SConfig CLoggerConfig::logConfigTable[CLoggerConfig::lm_num] = {
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::Main,                       true,  "main"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::EventPublisherManager,      true,  "eventPublisherManager"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::BaseQueue,                  true,  "baseQueue"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::BaseTask,                   true,  "baseTask"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::MessageQueue,               true,  "messageQueue"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::Hardware,                   true,  "hardware"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::Control,                    true,  "control"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::Communication,              true,  "communication"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::Wifi,                       true,  "wifi"},
    {CLoggerConfig::CLogLevel::Debug,        CLoggerConfig::CLogModule::HttpsClient,                true,  "httpsClient"},
};
