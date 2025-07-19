#include "CMeasQueue.h"

CMeasQueue::CMeasQueue(size_t aMaxSize, IStorage& aItsStorage) : 
    mMaxSize(aMaxSize),
    mItsStorage(aItsStorage),
    mLog(CLoggerConfig::CLogModule::MeasQueue)
{
    mQueue.reserve(mMaxSize);

    size_t counterSize = sizeof(mCounter);
    IStorage::EStorageStatus res = mItsStorage.read(mNamespace, mKeyCounter, static_cast<void*>(&mCounter), counterSize);

    if(res == IStorage::EStorageStatus::NOT_FOUND)
    {
        mLog.warning("%s: mCounter not found, initializing to 0", __FUNCTION__);
        mCounter = 0;
        res = mItsStorage.write(mNamespace, mKeyCounter, static_cast<void*>(&mCounter), counterSize);
        if (res != IStorage::EStorageStatus::OK)
        {
            mLog.error("%s: write failed with status: %d", __FUNCTION__, static_cast<int>(res));
            return;
        }
    }
    else if (res == IStorage::EStorageStatus::OK)
    {
        mLog.success("%s: mCounter read successfully: %zu", __FUNCTION__, mCounter);
    }
    else
    {
        mLog.error("%s: read failed with status: %d", __FUNCTION__, static_cast<int>(res));
        return;
    }

    if (mCounter == mMaxSize)
    {
        mIsQueueFull = true;
        mLog.success("%s: mCounter reached max size, queue is full", __FUNCTION__);
    }
}

CMeasQueue::~CMeasQueue()
{
}

bool CMeasQueue::isQueueFull() const
{
    return mIsQueueFull;
}

bool CMeasQueue::addMeasurement(SMeassurment& measurement)
{
    mCounter++;
    std::string number = "[" + std::to_string(mCounter) + "]";
    mLog.debug("%s: Adding measurement %s to queue, counter: %zu", __FUNCTION__, number.c_str(), mCounter);

    IStorage::EStorageStatus res = IStorage::EStorageStatus::ERROR;
    res = mItsStorage.write(mNamespace, (mKeyQueue + number), static_cast<void*>(&measurement), sizeof(SMeassurment));
    if (res != IStorage::EStorageStatus::OK)
    {
        mLog.error("%s: Failed to write measurement to storage with status: %d", __FUNCTION__, static_cast<int>(res));
        return false;
    }
    res = mItsStorage.write(mNamespace, mKeyCounter, static_cast<void*>(&mCounter), sizeof(mCounter));
    if (res != IStorage::EStorageStatus::OK)
    {
        mLog.error("%s: Failed to write measurement to storage with status: %d", __FUNCTION__, static_cast<int>(res));
        return false;
    }

    if (mCounter == mMaxSize)
    {
        mIsQueueFull = true;
        mLog.success("%s: Queue reached max size, now full", __FUNCTION__);
        for(size_t i = 0; i < mMaxSize; ++i)
        {
            std::string key = mKeyQueue + "[" + std::to_string(i + 1) + "]";
            SMeassurment data;
            size_t dataSize = sizeof(SMeassurment);
            res = mItsStorage.read(mNamespace, key, static_cast<void*>(&data), dataSize);
            if (res == IStorage::EStorageStatus::OK)
            {
                mLog.debug("%s: Measurement %zu read successfully from storage", __FUNCTION__, i + 1);
                mLog.debug("Measurement %zu - temperature: %.2f, humidity: %.2f, pressure: %.2f, voltage: %.2f",
                                    i + 1, data.mTemperature, data.mHumidity, data.mPressure, data.mVoltage);
                mQueue.push_back(data);
            }
            else
            {
                mLog.error("%s: Failed to read measurement from storage with status: %d", __FUNCTION__, static_cast<int>(res));
                return false;
            }
        }
        mCounter = 0;
        res = mItsStorage.write(mNamespace, mKeyCounter, static_cast<void*>(&mCounter), sizeof(mCounter));
        if (res != IStorage::EStorageStatus::OK)
        {
            mLog.error("%s: Failed to write counter to storage with status: %d", __FUNCTION__, static_cast<int>(res));
            return false;
        }
    }
    else if (mCounter > mMaxSize)
    {
        mLog.error("%s: Counter exceeded max size, resetting to 0", __FUNCTION__);
        mCounter = 0;
        mIsQueueFull = false;
        mQueue.clear();
    }

    size_t counterSize = sizeof(mCounter);
    res = mItsStorage.write(mNamespace, mKeyCounter, static_cast<void*>(&mCounter), counterSize);
    
    if (res != IStorage::EStorageStatus::OK)
    {
        mLog.error("%s: Failed to write counter to storage with status: %d", __FUNCTION__, static_cast<int>(res));
        return false;
    }

    return true;
}

bool CMeasQueue::readMeasurements(SMeassurmentMultiple& outMeasurements)
{
    mLog.debug("%s: Reading measurements from queue", __FUNCTION__);

    if(!mIsQueueFull || mQueue.empty())
    {
        mLog.debug("%s: Queue is not full, measurements may not be complete", __FUNCTION__);
        return false;
    }

    for (size_t i = 0; i < mQueue.size() && i < 10; ++i)
    {
        outMeasurements.mTemperature[i] = mQueue[i].mTemperature;
        outMeasurements.mHumidity[i] = mQueue[i].mHumidity;
        outMeasurements.mPressure[i] = mQueue[i].mPressure;
        outMeasurements.mVoltage[i] = mQueue[i].mVoltage;
    }

    return true;
}

