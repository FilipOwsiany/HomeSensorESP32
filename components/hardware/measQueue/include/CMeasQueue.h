#ifndef __CMEASQUEUE_H__
#define __CMEASQUEUE_H__

#include "commonStd.h"
#include "commonData.h"

#include "CLoggerHelper.h"

#include "IStorage.h"

class CMeasQueue
{
private:
    const std::string mNamespace = "hardware";
    const std::string mKeyCounter = "counter";
    const std::string mKeyQueue = "queue";
    const size_t mMaxSize;

    size_t mCounter = 0;
    bool mIsQueueFull = false;
    std::vector<SMeassurment> mQueue;
    IStorage& mItsStorage;

    CLoggerHelper mLog;
public:
    CMeasQueue(size_t aMaxSize, IStorage& aItsStorage);
    ~CMeasQueue();

    bool isQueueFull() const;
    bool addMeasurement(SMeassurment& measurement);
    bool readMeasurements(SMeassurmentMultiple& outMeasurements);
};

#endif //__CMEASQUEUE_H__