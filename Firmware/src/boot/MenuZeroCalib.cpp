#include "boot/MenuZeroCalib.hpp"
#include "ui/Icons.hpp"
#include "ui/Draw.hpp"
#include "common/config.hpp"
#include "common/Log.hpp"
#include "common/NVS.hpp"
#include "drivers/MotorDriver.hpp"
#include <esp_system.h>
#include <freertos/FreeRTOS.h>

bool MenuZeroCalib::onBack()
{
    // write flag in NVS
    NVS::Handle* nvshandle;
    if (Error err = NVS::Open("boot", &nvshandle); err != Error::None)
    {
        LOG_ERROR(TAG, "Error opening NVS 'boot' namespace : %s", ErrorToString(err));
        return true; // even if we fail to save, we still want to go back
    }
    nvshandle->set<bool>("skip_zerocalib", true);
    delete nvshandle;

    skipping = true;

    // It's not good to draw on callbacks, but we need to display the reboot message
    ScreenDriver::Clear();
    Draw::Text(16, 28, "Skipping ...");
    ScreenDriver::Upload();
    vTaskDelay(pdMS_TO_TICKS(100)); // wait a bit to be sure it updates

    esp_restart(); // restart to apply zero-calibration flag
    return false;
}

bool MenuZeroCalib::onSelect()
{
    if (calibrated || calibrating) return false;
    calibrating = true;
    motorIndex = 0;
    triggerRender();
    return true;
}

bool MenuZeroCalib::onNext()
{
    return false;
}

bool MenuZeroCalib::onPrev()
{
    return false;
}


void MenuZeroCalib::onShow()
{
    triggerRender();
}

void MenuZeroCalib::onHide()
{ 
}

void MenuZeroCalib::onRender()
{
    if (skipping) return; // if we're skipping, we don't want to render anything

    if (calibrated)
    {
        const char* text = "Calibrated!";
        uint16_t width = Draw::GetTextWidth(text);
        Draw::Text(ScreenDriver::info.width / 2 - width / 2, 0, text);
        Draw::Text(0, 12, "All motors have been set.       You can turn offthe robot.");
    }
    else if (calibrating)
    {
        Draw::Text(0, 0, " Calibrating... ");
        char text[16];
        sprintf(text, "Motor %d/16", motorIndex);
        uint16_t width = Draw::GetTextWidth(text);
        Draw::Text(ScreenDriver::info.width / 2 - width / 2, ScreenDriver::info.height / 2 - 4, text);
    }
    else
    {
        Draw::Text(0, 0, "Zero-Calibration");
        Draw::Text(0, 12, "All motors are  going to be set to their defaultorientation.");
        Draw::Text(ScreenDriver::info.width - 12 - 16, ScreenDriver::info.height - 9, "Ok");
        Draw::Text(11, ScreenDriver::info.height - 9, "Skip");
        Draw::Blit(ScreenDriver::info.width - 9, ScreenDriver::info.height - 9, 8, 8, (uint8_t*) Icons::ChevronRight);
        Draw::Blit(0, ScreenDriver::info.height - 9, 8, 8, (uint8_t*) Icons::ChevronLeft);
    }
}

void MenuZeroCalib::onUpdate()
{
    // only update every second
    if (counter++ >= SCREEN_REFRESH_RATE)
    {
        counter = 0;
    } else return;

    if (!calibrating) return;
    if (motorIndex >= 16) calibrated = true;
    else 
    {
        if (Error err = MotorDriver::SetPWM(motorIndex, MotorDriver::MS_TO_PWM(1.5f)); err != Error::None)
        {
            LOG_ERROR(TAG, "MotorDriver::SetPWM failed");
        }
        if (Error err = MotorDriver::SendData(); err != Error::None)
        {
            LOG_ERROR(TAG, "MotorDriver::SendData failed");
        }
        motorIndex++;
    }

    // if calibrated, save to NVS
    if (calibrated)
    {
        NVS::Handle* nvshandle;
        if (Error err = NVS::Open("boot", &nvshandle); err != Error::None)
        {
            LOG_ERROR(TAG, "Error opening NVS 'boot' namespace : %s", ErrorToString(err));
            return;
        }
        nvshandle->set<bool>("skip_zerocalib", true);

        // Disable all motors after calibration, to avoid jumps on next boot
        MotorDriver::DisableAllMotors();
    }

    if (!skipping) triggerRender();
}
