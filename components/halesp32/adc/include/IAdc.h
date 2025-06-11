#ifndef __IADC_H__
#define __IADC_H__

#include "commonStd.h"

using adcVoltage = int;
using adcRaw =  int;

class IAdc
{
private:

public:
    virtual ~IAdc() = default;

    virtual adcVoltage readOneShot(void) = 0;
    virtual adcVoltage readAvrage(uint16_t repeat = 100) = 0;
};

#endif //__IADC_H__

