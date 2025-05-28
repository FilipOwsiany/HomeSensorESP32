#include "CAdc.h"

#include "CLogger.h"

CAdc::CAdc(adc_unit_t unit, adc_channel_t channel) : mUnit(unit), mChannel(channel)
{
    CLogger::log(CLoggerModule::Adc, CLoggerLevel::Debug, "CAdc constructor called");
    CLogger::log(CLoggerModule::Adc, CLoggerLevel::Debug, "Unit: %d, Channel: %d", unit, channel);
    adc_cali_line_fitting_config_t caliConfig = {};
    caliConfig.unit_id = unit;
    caliConfig.atten = ADC_ATTEN_DB_12;
    caliConfig.bitwidth = ADC_BITWIDTH_12;

	ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&caliConfig, &mAdcCali));

	adc_oneshot_unit_init_cfg_t oneshotUnitConfig = {};
	oneshotUnitConfig.unit_id = unit;
	oneshotUnitConfig.ulp_mode = ADC_ULP_MODE_DISABLE;

	ESP_ERROR_CHECK(adc_oneshot_new_unit(&oneshotUnitConfig, &mAdcHandle));

	adc_oneshot_chan_cfg_t config = {};
    config.atten= ADC_ATTEN_DB_12;
    config.bitwidth = ADC_BITWIDTH_12;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(mAdcHandle, channel, &config));
}

CAdc::~CAdc()
{
    
}

adcVoltage CAdc::readOneShot(void)
{    
    esp_err_t ret = ESP_FAIL;
    adcRaw rawRead = 0;
    do
    {
        ret = adc_oneshot_read(mAdcHandle, mChannel, static_cast<int*>(&rawRead));
    } while (ret == ESP_ERR_INVALID_STATE);
    adcVoltage voltage = 0;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(mAdcCali, rawRead, &voltage));
    CLogger::log(CLoggerModule::Adc, CLoggerLevel::Debug, "ADC Read: %d, Voltage: %d", rawRead, voltage);
    return voltage;
}

adcVoltage CAdc::readAvrage(uint16_t repeat)
{
    adcRaw rawSum = 0;
    for (uint16_t cnt = 0; cnt < repeat; cnt++)
    {
        esp_err_t ret = ESP_FAIL;
        adcRaw rawRead = 0;
        do
        {
            ret = adc_oneshot_read(mAdcHandle, mChannel, static_cast<int*>(&rawRead));
        } while (ret == ESP_ERR_INVALID_STATE);
        rawSum += rawRead;
    }
    adcVoltage voltage = 0;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(mAdcCali, rawSum / repeat, &voltage));
    CLogger::log(CLoggerModule::Adc, CLoggerLevel::Debug, "ADC Read Average: %d, Voltage: %d", rawSum / repeat, voltage);
    return voltage;
}