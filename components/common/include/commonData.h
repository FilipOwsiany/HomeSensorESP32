#ifndef __COMMONDATA_H__
#define __COMMONDATA_H__

#include "commonStd.h"

struct SMeassurment
{
    float mTemperature;
    float mHumidity;
    float mPressure;

    SMeassurment() : mTemperature(0.0f), mHumidity(0.0f), mPressure(0.0f) {}
    SMeassurment(float temperature, float humidity, float pressure)
        : mTemperature(temperature), mHumidity(humidity), mPressure(pressure) {}
    SMeassurment(const SMeassurment& other)
        : mTemperature(other.mTemperature), mHumidity(other.mHumidity), mPressure(other.mPressure) {}
    SMeassurment& operator=(const SMeassurment& other)
    {
        if (this != &other)
        {
            mTemperature = other.mTemperature;
            mHumidity = other.mHumidity;
            mPressure = other.mPressure;
        }
        return *this;
    }
};

struct SSettings
{
    uint32_t mWakeupTimeout;
    uint32_t mSleepTimeout;
    uint32_t mMeasurementInterval;

    SSettings() : mWakeupTimeout(0), mSleepTimeout(0), mMeasurementInterval(0) {}
    SSettings(uint32_t wakeupTimeout, uint32_t sleepTimeout, uint32_t measurementInterval)
        : mWakeupTimeout(wakeupTimeout), mSleepTimeout(sleepTimeout), mMeasurementInterval(measurementInterval) {}
    SSettings(const SSettings& other)
        : mWakeupTimeout(other.mWakeupTimeout), mSleepTimeout(other.mSleepTimeout), mMeasurementInterval(other.mMeasurementInterval) {}
    SSettings& operator=(const SSettings& other)
    {
        if (this != &other)
        {
            mWakeupTimeout = other.mWakeupTimeout;
            mSleepTimeout = other.mSleepTimeout;
            mMeasurementInterval = other.mMeasurementInterval;
        }
        return *this;
    }
};

#endif //__COMMONDATA_H__