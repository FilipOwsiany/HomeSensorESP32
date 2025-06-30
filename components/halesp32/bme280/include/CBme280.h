#ifndef __CBME280_H__
#define __CBME280_H__

#include "bme280.h"
#include "bmx280.h"
#include "bmx280_bits.h"

#include "CBaseTask.h"

#include "IBme280.h"

class CBme280 : public IBme280, public CBaseTask
{
public:
    enum class Bmx280TemperatureOversampling {
        NONE = 0x0,
        X1,
        X2,
        X4,
        X8,
        X16
    };

    enum class Bmx280PressureOversampling {
        NONE = 0x0,
        X1,
        X2,
        X4,
        X8,
        X16
    };

    enum class Bme280HumidityOversampling {
        NONE = 0x0,
        X1,
        X2,
        X4,
        X8,
        X16
    };

    enum class Bmx280StandbyTime {
        STANDBY_0M5 = 0x0,
        STANDBY_62M5,
        STANDBY_125M,
        STANDBY_250M,
        STANDBY_500M,
        STANDBY_1000M,
        STANDBY_10M,
        STANDBY_20M,
        STANDBY_2000M,
        STANDBY_4000M
    };

    enum class Bmx280IirFilter {
        NONE = 0x0,
        X1,
        X2,
        X4,
        X8,
        X16
    };

    enum class Bmx280Mode {
        SLEEP = 0,
        FORCE = 1,
        CYCLE = 3
    };
private:
    i2c_config_t mI2CConfig;
    bmx280_config_t mBmxConfig;
    bmx280_t* mBmx280;

    Bmx280Mode mMode;
    Bmx280TemperatureOversampling mTempOversampling;
    Bmx280PressureOversampling mPressureOversampling;
    Bme280HumidityOversampling mHumidityOversampling;
    Bmx280StandbyTime mStandbyTime;
    Bmx280IirFilter mIirFilter;
public:
    CBme280(Bmx280Mode mode = Bmx280Mode::FORCE,
            Bmx280TemperatureOversampling tempOversampling = Bmx280TemperatureOversampling::X1,
            Bmx280PressureOversampling pressureOversampling = Bmx280PressureOversampling::X1,
            Bme280HumidityOversampling humidityOversampling = Bme280HumidityOversampling::X1,
            Bmx280StandbyTime standbyTime = Bmx280StandbyTime::STANDBY_1000M,
            Bmx280IirFilter iirFilter = Bmx280IirFilter::NONE);
    ~CBme280();
    bool read(SBme280& bme280Data) override;
    bool readAvrage(SBme280& bme280Data, uint8_t count) override;

};

#endif //__CBME280_H__

