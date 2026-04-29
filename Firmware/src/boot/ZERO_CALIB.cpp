#include "boot/BootManager.hpp"
#include "common/Log.hpp"
#include "common/LED.hpp"
#include "common/I2C.hpp"
#include "common/NVS.hpp"
#include "ui/Menus.hpp"
#include "ui/Draw.hpp"
#include "ui/Icons.hpp"
#include "boot/MenuZeroCalib.hpp"
#include "drivers/ScreenDriver.hpp"
#include "drivers/MotorDriver.hpp"
#include <freertos/FreeRTOS.h>

namespace BootManager
{
    void boot_ZERO_CALIB()
    {
        // Initialize LED module for error display
        if (Error err = LED::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize LED module");
            return;
        }

        // Initialize I2C for screen and motor driver
        if (Error err = I2C::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize I2C module");
            return;
        }

        // Initialize Screen and Menu system for user interface
        if (Error err = ScreenDriver::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize ScreenDriver module");
            LED::LoopErrorCode(ErrorCode::ScreenInitFailed);
            return;
        }
        if (Error err = Menus::Init(); err != Error::None)
        {
            LOG_ERROR(TAG, "Failed to initialize Menus module");
            LED::LoopErrorCode(ErrorCode::ScreenInitFailed);
            return;
        }

        // Display splash screen
        Menus::SetCurrentMenu(Menus::GetMenuSplash());

        // Initialize Motor Driver for motor control
        // if (Error err = MotorDriver::Init(); err != Error::None)
        // {
        //     LOG_ERROR(TAG, "Failed to initialize MotorDriver module");
        //     LED::LoopErrorCode(ErrorCode::DriverInitFailed);
        //     return;
        // }

        // Show zero-calib menu
        MenuZeroCalib menu;
        Menus::SetCurrentMenu(&menu);

        // infinite loop to keep menu variable alive
        while (true) { vTaskDelay(pdMS_TO_TICKS(100)); };
    }
}