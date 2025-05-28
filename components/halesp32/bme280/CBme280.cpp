#include "CBme280.h"

#include "driver/i2c.h"

#include "CLogger.h"

CBme280::CBme280(Bmx280Mode mode,
                 Bmx280TemperatureOversampling tempOversampling,
                 Bmx280PressureOversampling pressureOversampling,
                 Bme280HumidityOversampling humidityOversampling,
                 Bmx280StandbyTime standbyTime,
                 Bmx280IirFilter iirFilter)
{
    mI2CConfig = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_17,
        .scl_io_num = GPIO_NUM_16,
        .sda_pullup_en = false,
        .scl_pullup_en = false,
        .master = {
            .clk_speed = 100000
        }
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &mI2CConfig));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

    mBmx280 = bmx280_create(I2C_NUM_0);

    if (!mBmx280) 
    { 
        CLogger::log(CLoggerModule::Bme280, CLoggerLevel::Error, "Failed to create BMX280 instance");
        return;
    }

    ESP_ERROR_CHECK(bmx280_init(mBmx280));

    mBmxConfig.t_sampling = static_cast<bmx280_tsmpl_t>(tempOversampling);
    mBmxConfig.p_sampling = static_cast<bmx280_psmpl_t>(pressureOversampling);
    mBmxConfig.h_sampling = static_cast<bme280_hsmpl_t>(humidityOversampling);
    mBmxConfig.t_standby = static_cast<bmx280_tstby_t>(standbyTime);
    mBmxConfig.iir_filter = static_cast<bmx280_iirf_t>(iirFilter);

    ESP_ERROR_CHECK(bmx280_configure(mBmx280, &mBmxConfig));
}

CBme280::~CBme280()
{
    bmx280_close(mBmx280);
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_NUM_0));
}

bool CBme280::read(SBme280& bme280Data)
{
    if (!mBmx280) 
    {
        CLogger::log(CLoggerModule::Bme280, CLoggerLevel::Error, "BMX280 instance is not initialized");
        return false;
    }

    ESP_ERROR_CHECK(bmx280_setMode(mBmx280, static_cast<bmx280_mode_t>(mMode)));
    do 
    {
        taskDelay(pdMS_TO_TICKS(1));
    } 
    while(bmx280_isSampling(mBmx280));

    ESP_ERROR_CHECK(bmx280_readoutFloat(mBmx280, 
                                        &bme280Data.temperature, 
                                        &bme280Data.pressure, 
                                        &bme280Data.humidity));

    CLogger::log(CLoggerModule::Bme280, CLoggerLevel::ImportantInfo, "Read Values: temperature = %f, pressure = %f, humidity = %f", 
                bme280Data.temperature, bme280Data.pressure, bme280Data.humidity);
    return true;
}