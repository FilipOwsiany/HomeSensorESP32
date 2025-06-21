#include "CHalEspressifFactory.h"

#include "CAdc.h"
#include "CBme280.h"
#include "CWifi.h"

IAdc& CHalEspressifFactory::createAdc(void) 
{
    static CAdc adc(CAdc::EAdcUnit::UNIT_1, 
                    CAdc::EAdcChannel::CHANNEL_6);
    return static_cast<IAdc&>(adc);
}

void CHalEspressifFactory::destroyAdc(void) 
{
    // No action needed for static instance
}

IBme280& CHalEspressifFactory::createBme280(void) 
{
    static CBme280 bme280(CBme280::Bmx280Mode::FORCE,
                          CBme280::Bmx280TemperatureOversampling::X16,
                          CBme280::Bmx280PressureOversampling::X4,
                          CBme280::Bme280HumidityOversampling::X4,
                          CBme280::Bmx280StandbyTime::STANDBY_20M,
                          CBme280::Bmx280IirFilter::X16);
    return static_cast<IBme280&>(bme280);
}

void CHalEspressifFactory::destroyBme280(void) 
{
    // No action needed for static instance
}

IWifi& CHalEspressifFactory::createWifi(void) 
{
    static CWifi& wifi = CWifi::getInstance();
    return static_cast<IWifi&>(wifi);
}

void CHalEspressifFactory::destroyWifi(void) 
{
   // No action needed for static instance
}