#include "CLowPower.h"

#include "esp_pm.h"
#include "esp_sleep.h"
#include "esp_cpu.h"
#include "esp_rom_sys.h"
#include "esp_wake_stub.h"

CLowPower& CLowPower::getInstance()
{
    static CLowPower instance;
    return instance;
}

void CLowPower::enterDeepSleep(uint64_t aSleepTimeUs)
{
    esp_sleep_enable_timer_wakeup(aSleepTimeUs);
    printf("Entering deep sleep for %llu microseconds\n", aSleepTimeUs);
    esp_deep_sleep_start();
}

void CLowPower::configureLowPowerMode(uint8_t aMaxClockFrequency, 
                                      uint8_t aMinClockFrequency, 
                                      bool aEnableLightSleep)
{
    esp_pm_config_t poweManagmentConfig = {
        .max_freq_mhz = aMaxClockFrequency,
        .min_freq_mhz = aMinClockFrequency,
        .light_sleep_enable = aEnableLightSleep
    };

    printf("Configuring low power mode: max_freq_mhz=%d, min_freq_mhz=%d, light_sleep_enable=%d\n",
           aMaxClockFrequency, aMinClockFrequency, aEnableLightSleep);
    esp_err_t ret = esp_pm_configure(&poweManagmentConfig);
}

