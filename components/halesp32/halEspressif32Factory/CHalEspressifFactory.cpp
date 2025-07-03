#include "CHalEspressifFactory.h"

#include "CAdc.h"
#include "CBme280.h"
#include "CWifi.h"
#include "CHttpsClient.h"

IAdc& CHalEspressifFactory::createAdc(void) 
{
    return static_cast<IAdc&>(*new CAdc(    
                                            CAdc::EAdcUnit::UNIT_1, 
                                            CAdc::EAdcChannel::CHANNEL_6)
                            );
}

void CHalEspressifFactory::destroyAdc(void) 
{
    // No action needed for static instance
}

IBme280& CHalEspressifFactory::createBme280(void) 
{
    return static_cast<IBme280&>(*new CBme280(  
                                                CBme280::Bmx280Mode::FORCE,
                                                CBme280::Bmx280TemperatureOversampling::X16,
                                                CBme280::Bmx280PressureOversampling::X4,
                                                CBme280::Bme280HumidityOversampling::X4,
                                                CBme280::Bmx280StandbyTime::STANDBY_20M,
                                                CBme280::Bmx280IirFilter::X16)
                        );
}

void CHalEspressifFactory::destroyBme280(void) 
{
    // No action needed for static instance
}

IWifi& CHalEspressifFactory::createWifi(void) 
{
    return static_cast<IWifi&>(CWifi::getInstance());
}

void CHalEspressifFactory::destroyWifi(void) 
{
   // No action needed for static instance
}

// IHttpsClient& CHalEspressifFactory::createHttpsClient(void) 
// {
//     return static_cast<IHttpsClient&>(* new CHttpsClient);
// }

// void CHalEspressifFactory::destroyHttpsClient(void) 
// {
//     // No action needed for static instance
// }