#include "commonStd.h"

class CLowPower
{
public:
    static CLowPower& getInstance();
    CLowPower(const CLowPower&) = delete;
    CLowPower& operator=(const CLowPower&) = delete;
private:
    CLowPower() = default;
    ~CLowPower() = default;
public:
    void enterDeepSleep(uint64_t aSleepTimeUs);
    void configureLowPowerMode(uint8_t aMaxClockFrequency, 
                               uint8_t aMinClockFrequency, 
                               bool aEnableLightSleep = true);
};

