#ifndef __CADC_H__
#define __CADC_H__

#include "commonStd.h"

#include "IAdc.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

using adcVoltage = int;
using adcRaw =  int;

class CAdc : public IAdc
{
public:
    enum class EAdcUnit 
    {
        UNIT_1,
        UNIT_2
    };

    enum class EAdcChannel 
    {
        CHANNEL_0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
        CHANNEL_6,
        CHANNEL_7,
        CHANNEL_8,
        CHANNEL_9
    };
private:
    CAdc::EAdcUnit              mUnit;
    CAdc::EAdcChannel           mChannel;
    
    adc_cali_handle_t           mAdcCali;
    adc_oneshot_unit_handle_t   mAdcHandle;

    const float                 VOLTAGE_DIVIDER_RATIO = 2.0f;
public:
    CAdc(CAdc::EAdcUnit unit, CAdc::EAdcChannel channel);
    ~CAdc();
    
    adcVoltage readOneShot(void) override;
    adcVoltage readAvrage(uint16_t repeat = 100) override;
};

#endif //__CADC_H__

