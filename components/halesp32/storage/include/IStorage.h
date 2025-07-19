#ifndef __ISTORAGE_H__
#define __ISTORAGE_H__

#include "commonStd.h"

class IStorage
{
private:

public:

    enum class EStorageStatus
    {
        OK,
        ERROR,
        NOT_FOUND,
        INVALID_HANDLE,
        INVALID_NAME,
        INVALID_LENGTH,
        INVALID_ARGUMENT,
        NO_MEMORY,
        NOT_INITIALIZED,
        NOT_ENOUGH_SPACE,
        READ_ONLY,
        TYPE_MISMATCH
    };

    virtual ~IStorage() = default;

    virtual EStorageStatus read(
        const std::string& aNamespace,
        const std::string& aKey,
        void*           outData,
        size_t&         ioDataSize) = 0;

    virtual EStorageStatus write(
        const std::string& aNamespace,
        const std::string& aKey,
        const void*     inData,
        size_t          inDataSize) = 0;
};

#endif //__CSTORAGE_H__