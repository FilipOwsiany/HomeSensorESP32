#ifndef __SBME280_H__
#define __SBME280_H__

struct SBme280
{
    float temperature;
    float pressure;
    float humidity;
    
    SBme280()
        : temperature(0.0f), pressure(0.0f), humidity(0.0f) {}
    SBme280(float temp, float press, float hum)
        : temperature(temp), pressure(press), humidity(hum) {}
    SBme280(const SBme280& other)
        : temperature(other.temperature), pressure(other.pressure), humidity(other.humidity) {}
    SBme280& operator=(const SBme280& other)
    {
        if (this != &other) {
            temperature = other.temperature;
            pressure = other.pressure;
            humidity = other.humidity;
        }
        return *this;
    }
};


#endif //__SBME280_H__
