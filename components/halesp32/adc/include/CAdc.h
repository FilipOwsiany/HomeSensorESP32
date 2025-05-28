#ifndef __CADC_H__
#define __CADC_H__

#include "commonStd.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

using adcVoltage = int;
using adcRaw =  int;

class CAdc
{
private:
    adc_unit_t mUnit;
    adc_channel_t mChannel;
    adc_cali_handle_t mAdcCali;
    adc_oneshot_unit_handle_t mAdcHandle;
public:
    CAdc(adc_unit_t unit, adc_channel_t channel);
    ~CAdc();
    adcVoltage readOneShot(void);
    adcVoltage readAvrage(uint16_t repeat = 100);
};

#endif //__CADC_H__

