#pragma once
#include "common/utils.hpp"
#include "common/NVS.hpp"

class UpdateManager
{
public:
    constexpr static const char* TAG = "UpdateManager";

    enum class Status: uint8_t {
        Done = 0,
        Fetching,
        DownloadingFirmware,
        DownloadingFilesystem,
        UpdatingFirmware,
        UpdatingFilesystem,
        VerifyingFirmware,
        Rebooting,
        ErrorUnreachable,
        ErrorInvalidJson,
        ErrorEmptyResponse,
        ErrorFirmwareUpdateFailed,
        ErrorFilesystemUpdateFailed,
        ErrorUnknown,
        ErrorPartitionNotFound,
        ErrorHTTPClient,
        ErrorOutOfBounds,
        ErrorEraseStorage,
    };

    UpdateManager();

    /**
     * @brief Initializes the Update Manager.
     * @return Error code indicating success or failure.
     * @note If an update was just applied, this function will trigger necessary verification steps, and may restart the system.
     */
    Error init();

    /**
     * @brief Deinitializes the Update Manager.
     * @return Error code indicating success or failure.
     */
    Error deinit();

    /**
     * @brief Checks for firmware updates from the server.
     * @return Error code indicating success or failure.
     * @note This launches the check in an other task, check getStatus and getProgress for info.
     */
    Error checkForUpdates();
    
    Error verifyFirmware();

    /**
     * @brief Downloads and installs a new firmware (stored using previous checkForUpdate() call).
     * @return Error code indicating success or failure.
     * @note This launches the check in an other task, check getStatus and getProgress for info.
     */
    Error downloadAndApplyFirmwareUpdate();
    
    /**
     * @brief Downloads and installs a new filesystem (stored using previous checkForUpdate() call).
     * @return Error code indicating success or failure.
     * @note This launches the check in an other task, check getStatus and getProgress for info.
     */
    Error downloadAndApplyFilesystemUpdate();

    bool isUpdateAvailable() const { return updateAvailable; }

    const char* getLatestVersion() const { return latestVersion; }

    Status getStatus() { return status; }

    float getProgress() { return progress; }

    /// @brief INTERNAL, do not execute by hand
    void run_update_task();

    /// @brief INTERNAL, do not execute by hand
    void run_download_firmware_task();
    
    /// @brief INTERNAL, do not execute by hand
    void run_download_filesystem_task();

private:
    NVS::Handle* nvsHandle_ptr;

    bool updateAvailable = false;
    bool isFilesystemUpdatePending = false;
    char* latestVersion = nullptr;
    char* firmwareDownloadUrl = nullptr;
    char* filesystemDownloadUrl = nullptr;

    float progress = 0.f;
    Status status = Status::Done;
};
