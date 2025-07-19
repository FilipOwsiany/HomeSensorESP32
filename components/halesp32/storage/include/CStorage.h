#ifndef __CSTORAGE_H__
#define __CSTORAGE_H__

#include "commonStd.h"

#include "IStorage.h"

class CStorage : public IStorage
{
public:
    CStorage() = default;
    ~CStorage() override = default;

    EStorageStatus read(
        const std::string& aNamespace,
        const std::string& aKey,
        void*           outData,
        size_t&         ioDataSize) override;

    EStorageStatus write(
        const std::string& aNamespace,
        const std::string& aKey,
        const void*     inData,
        size_t          inDataSize) override;
};

#endif //__CSTORAGE_H__