#include "CStorage.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "IStorage.h"

static  IStorage::EStorageStatus mapEspError(esp_err_t err)
{
    switch (err) {
        case ESP_OK:                         return IStorage::EStorageStatus::OK;
        case ESP_FAIL:                       return IStorage::EStorageStatus::ERROR;
        case ESP_ERR_NVS_NOT_INITIALIZED:    return IStorage::EStorageStatus::NOT_INITIALIZED;
        case ESP_ERR_NVS_PART_NOT_FOUND:     return IStorage::EStorageStatus::ERROR;
        case ESP_ERR_NVS_NOT_FOUND:          return IStorage::EStorageStatus::NOT_FOUND;
        case ESP_ERR_NVS_INVALID_NAME:       return IStorage::EStorageStatus::INVALID_NAME;
        case ESP_ERR_NO_MEM:                 return IStorage::EStorageStatus::NO_MEMORY;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:   return IStorage::EStorageStatus::NOT_ENOUGH_SPACE;
        case ESP_ERR_NVS_INVALID_HANDLE:     return IStorage::EStorageStatus::INVALID_HANDLE;
        case ESP_ERR_NVS_INVALID_LENGTH:     return IStorage::EStorageStatus::INVALID_LENGTH;
        case ESP_ERR_NVS_READ_ONLY:          return IStorage::EStorageStatus::READ_ONLY;
        case ESP_ERR_NVS_REMOVE_FAILED:      return IStorage::EStorageStatus::ERROR;
        case ESP_ERR_NVS_VALUE_TOO_LONG:     return IStorage::EStorageStatus::INVALID_LENGTH;
        default:                             return IStorage::EStorageStatus::ERROR;
    }
}


IStorage::EStorageStatus CStorage::read(
    const std::string& aNamespace,
    const std::string& aKey,
    void*           outData,
    size_t&         ioDataSize)
{
    nvs_handle_t handle;
    esp_err_t    err = nvs_open(aNamespace.c_str(), NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return mapEspError(err);
    }

    // 1) dowiedz się, ile bajtów trzeba wczytać
    size_t required = 0;
    err = nvs_get_blob(handle, aKey.c_str(), nullptr, &required);
    if (err != ESP_OK && err != ESP_ERR_NVS_INVALID_LENGTH) {
        nvs_close(handle);
        return mapEspError(err);
    }

    if (ioDataSize < required) {
        nvs_close(handle);
        return EStorageStatus::INVALID_LENGTH;
    }

    // 2) właściwy odczyt
    err = nvs_get_blob(handle, aKey.c_str(), outData, &ioDataSize);
    nvs_close(handle);
    return mapEspError(err);
}

IStorage::EStorageStatus CStorage::write(
    const std::string& aNamespace,
    const std::string& aKey,
    const void*     inData,
    size_t          inDataSize)
{
    nvs_handle_t handle;
    esp_err_t    err = nvs_open(aNamespace.c_str(), NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return mapEspError(err);
    }

    err = nvs_set_blob(handle, aKey.c_str(), inData, inDataSize);
    if (err != ESP_OK) {
        nvs_close(handle);
        return mapEspError(err);
    }

    err = nvs_commit(handle);
    nvs_close(handle);
    return mapEspError(err);
}
