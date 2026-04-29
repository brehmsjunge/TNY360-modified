#include "boot/MenuZeroCalib.hpp"
#include "ui/Icons.hpp"
#include "ui/Draw.hpp"
#include "common/config.hpp"
#include "common/Log.hpp"
#include "common/NVS.hpp"
#include "drivers/MotorDriver.hpp"

bool MenuZeroCalib::onBack()
{
    return false;
}

bool MenuZeroCalib::onSelect()
{
    return false;
}

bool MenuZeroCalib::onNext()
{
    if (calibrated || calibrating) return false;
    calibrating = true;
    motorIndex = 0;
    triggerRender();
    return true;
}

bool MenuZeroCalib::onPrev()
{
    // skipping to final state
    calibrating = true;
    calibrated = true;
    motorIndex = 16;
    triggerRender();
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
            LOG_ERROR("ZeroCalib", "MotorDriver::SetPWM failed");
        }
        if (Error err = MotorDriver::SendData(); err != Error::None)
        {
            LOG_ERROR("ZeroCalib", "MotorDriver::SendData failed");
        }
        motorIndex++;
    }

    // if calibrated, save to NVS
    if (calibrated)
    {
        NVS::Handle* nvshandle;
        if (Error err = NVS::Open("boot", &nvshandle); err != Error::None)
        {
            LOG_ERROR("ZeroCalib", "Error opening NVS 'boot' namespace : %s", ErrorToString(err));
            return;
        }
        nvshandle->set<bool>("zero-calib", true);
    }

    triggerRender();
}
