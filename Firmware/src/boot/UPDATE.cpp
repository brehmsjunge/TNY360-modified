#include "boot/BootManager.hpp"
#include "common/Log.hpp"
#include "common/LED.hpp"
#include "common/I2C.hpp"
#include "common/NVS.hpp"
#include "network/WiFiManager.hpp"
#include "ui/Menus.hpp"
#include "ui/Draw.hpp"
#include "ui/Icons.hpp"
#include "drivers/ScreenDriver.hpp"
#include "esp_ota_ops.h"
#include "network/UpdateManager.hpp"
#include <freertos/FreeRTOS.h>

namespace BootManager
{
    bool boot_UPDATE_needed()
    {
        // Check if firmware needs to be verified
        LOG_DEBUG(TAG, "Checking if firmware verification is needed...");
        const esp_partition_t* running = esp_ota_get_running_partition();
        esp_ota_img_states_t ota_state;
        if (esp_ota_get_state_partition(running, &ota_state) != ESP_OK)
        {
            LOG_ERROR(TAG, "Failed to get OTA state for running partition");
            LOG_ERROR(TAG, "Rebooting ...");
            esp_restart();
        }

        return ota_state == ESP_OTA_IMG_PENDING_VERIFY;
    }

    void boot_UPDATE()
    {
        // Initialize LED module for error display
        LOG_DEBUG(TAG, "Initializing LED");
        if (Error err = LED::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize LED module");
            esp_ota_mark_app_invalid_rollback_and_reboot();
            return;
        }

        // Initialize I2C for screen
        if (Error err = I2C::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize I2C module");
            return;
        }

        // Initialize Screen and Menu system for user interface
        LOG_DEBUG(TAG, "Initializing ScreenDriver");
        if (Error err = ScreenDriver::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize ScreenDriver module");
            esp_ota_mark_app_invalid_rollback_and_reboot();
            return;
        }

        // Display a "Verifying firmware" message
        ScreenDriver::Clear();
        Draw::Text(0, 20, "Running firmware");
        Draw::Text(12, 24, "Checks ...");
        ScreenDriver::Upload();

        UpdateManager man;
        if (Error err = man.init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize update manager.");
            esp_ota_mark_app_invalid_rollback_and_reboot();
        }

        if (Error err = man.verifyFirmware(); err != Error::None)
        {
            LOG_ERROR(TAG, "Firmware verification failed. Rolling back...");
            esp_ota_mark_app_invalid_rollback_and_reboot();
        }

        LOG_INFO(TAG, "No error during diagnostics, marking firmware as valid.");
        esp_ota_mark_app_valid_cancel_rollback();

        LOG_INFO(TAG, "Waiting for a Wi-Fi connection...");

        // Display a "Waiting for Wi-Fi connection" message
        ScreenDriver::Clear();
        Draw::Text(0, 0, "Waiting for a   Wi-Fi connection...");
        ScreenDriver::Upload();

        // Turn on Wi-Fi
        WiFiManager wifi;
        wifi.init();
        
        // Wait for wifi connection
        LOG_INFO(TAG, "Connecting...");
        ScreenDriver::Clear();
        Draw::Text(0, 0, "Connecting...");
        ScreenDriver::Upload();
        while (wifi.getState() == WiFiManager::State::Connecting) { vTaskDelay(pdMS_TO_TICKS(100)); }

        if (wifi.getState() != WiFiManager::State::Connected)
        {
            // Display an error message and reboot
            ScreenDriver::Clear();
            Draw::Text(0, 0, "Couldn't connectto Wi-Fi.");
            Draw::Text(0, 20, "Manually update the filesystem  later in the    System > Update settings.");
            ScreenDriver::Upload();
            vTaskDelay(pdMS_TO_TICKS(3000));
            esp_restart();
        }

        // Launch filesystem download
        LOG_INFO(TAG, "Downloading filesystem...");
        man.downloadAndApplyFilesystemUpdate();
        vTaskDelay(200);
        while (man.getStatus() == UpdateManager::Status::DownloadingFilesystem)
        {
            // Display the progress bar
            ScreenDriver::Clear();
            {
                const char* text = "Downloading";
                uint16_t width = Draw::GetTextWidth(text);
                Draw::Text(ScreenDriver::info.width / 2 - width / 2, 16, text);
            }
            {
                const char* text = "File system";
                uint16_t width = Draw::GetTextWidth(text);
                Draw::Text(ScreenDriver::info.width / 2 - width / 2, 28, text);
            }
            Draw::RectRounded(8, 40, ScreenDriver::info.width - 16, 16, 4);
            Draw::RectRounded(9, 41, ScreenDriver::info.width - 18, 14, 3, ScreenDriver::COLOR_BLACK);
            Draw::RectRounded(9, 41, (ScreenDriver::info.width - 18) * man.getProgress(), 14, 3);
            ScreenDriver::Upload();
            
            vTaskDelay(pdMS_TO_TICKS(50)); // little delay to lower framerate
        }

        // Say ok and reboot
        ScreenDriver::Clear();
        {
            const char* text = "Done!";
            uint16_t width = Draw::GetTextWidth(text);
            Draw::Text(ScreenDriver::info.width / 2 - width / 2, 20, text);
        }
        {
            const char* text = "Rebooting ...";
            uint16_t width = Draw::GetTextWidth(text);
            Draw::Text(ScreenDriver::info.width / 2 - width / 2, 32, text);
        }
        ScreenDriver::Upload();
        vTaskDelay(3000);
        esp_restart();
    }
}