#include "CLoggerConfig.h"

CLoggerConfig::SConfig CLoggerConfig::logConfigTable[CLoggerConfig::lm_num] = {
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Main,                       true,  "main"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::EventPublisherManager,      true,  "eventPublisherManager"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::BaseQueue,                  true,  "baseQueue"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::BaseSemaphore,              true,  "baseSemaphore"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::BaseTask,                   true,  "baseTask"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::MessageQueue,               true,  "messageQueue"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Hardware,                   true,  "hardware"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Control,                    true,  "control"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Communication,              true,  "communication"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Wifi,                       true,  "wifi"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::HttpsClient,                true,  "httpsClient"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Adc,                        true,  "Adc"},
    {CLoggerConfig::CLogLevel::ImportantInfo,        CLoggerConfig::CLogModule::Bme280,                     true,  "Bme280"},
};
