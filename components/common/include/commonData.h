#ifndef __COMMONDATA_H__
#define __COMMONDATA_H__

#include "commonStd.h"

struct SMeassurment
{
    float mTemperature;
    float mHumidity;
    float mPressure;
    float mVoltage;

    SMeassurment() : mTemperature(0.0f), mHumidity(0.0f), mPressure(0.0f), mVoltage(0.0f) {}
    SMeassurment(float temperature, float humidity, float pressure, float voltage)
        : mTemperature(temperature), mHumidity(humidity), mPressure(pressure), mVoltage(voltage) {}
    SMeassurment(const SMeassurment& other)
        : mTemperature(other.mTemperature), mHumidity(other.mHumidity), mPressure(other.mPressure), mVoltage(other.mVoltage) {}
    SMeassurment& operator=(const SMeassurment& other)
    {
        if (this != &other)
        {
            mTemperature = other.mTemperature;
            mHumidity = other.mHumidity;
            mPressure = other.mPressure;
            mVoltage = other.mVoltage;
        }
        return *this;
    }
};

struct SMeassurmentMultiple
{
    float mTemperature[10];
    float mHumidity[10];
    float mPressure[10];
    float mVoltage[10];

    SMeassurmentMultiple()
    {
        for (int i = 0; i < 10; ++i)
        {
            mTemperature[i] = 0.0f;
            mHumidity[i] = 0.0f;
            mPressure[i] = 0.0f;
            mVoltage[i] = 0.0f;
        }
    }

    SMeassurmentMultiple(const float temperature[10], const float humidity[10], const float pressure[10], const float voltage[10])
    {
        for (int i = 0; i < 10; ++i)
        {
            mTemperature[i] = temperature[i];
            mHumidity[i] = humidity[i];
            mPressure[i] = pressure[i];
            mVoltage[i] = voltage[i];
        }
    }

    SMeassurmentMultiple(const SMeassurmentMultiple& other)
    {
        for (int i = 0; i < 10; ++i)
        {
            mTemperature[i] = other.mTemperature[i];
            mHumidity[i] = other.mHumidity[i];
            mPressure[i] = other.mPressure[i];
            mVoltage[i] = other.mVoltage[i];
        }
    }

    SMeassurmentMultiple(const SMeassurment (&measurements)[10])
    {
        for (int i = 0; i < 10; ++i)
        {
            mTemperature[i] = measurements[i].mTemperature;
            mHumidity[i]    = measurements[i].mHumidity;
            mPressure[i]    = measurements[i].mPressure;
            mVoltage[i]     = measurements[i].mVoltage;
        }
    }

    SMeassurmentMultiple& operator=(const SMeassurmentMultiple& other)
    {
        if (this != &other)
        {
            for (int i = 0; i < 10; ++i)
            {
                mTemperature[i] = other.mTemperature[i];
                mHumidity[i] = other.mHumidity[i];
                mPressure[i] = other.mPressure[i];
                mVoltage[i] = other.mVoltage[i];
            }
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