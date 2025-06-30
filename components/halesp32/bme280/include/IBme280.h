#ifndef __IBME280_H__
#define __IBME280_H__

#include "SBme280.h"

class IBme280
{
public:
    virtual ~IBme280() = default;
    virtual bool read(SBme280& bme280Data) = 0;
    virtual bool readAvrage(SBme280& bme280Data, uint8_t count) = 0;
};

#endif //__IBME280_H__

