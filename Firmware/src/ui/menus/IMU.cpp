#include "ui/menus/IMU.hpp"
#include "ui/Icons.hpp"
#include "ui/Draw.hpp"
#include "common/config.hpp"
#include "drivers/IMUDriver.hpp"

MenuIMU::MenuIMU(Menu* parent)
    : Menu("IMU", parent, Icons::IMUMenu)
{
}

bool MenuIMU::onBack()
{
    return false;
}

bool MenuIMU::onSelect()
{
    return false;
}

bool MenuIMU::onNext()
{
    uint8_t count = static_cast<uint8_t>(Page::Count);
    currentPage = static_cast<Page>((static_cast<uint8_t>(currentPage) + 1) % count);
    triggerRender();
    return true;
}

bool MenuIMU::onPrev()
{
    uint8_t count = static_cast<uint8_t>(Page::Count);
    currentPage = static_cast<Page>((static_cast<uint8_t>(currentPage) + count - 1) % count);
    triggerRender();
    return true;
}


void MenuIMU::onShow()
{
}

void MenuIMU::onHide()
{ 
}

void MenuIMU::onRender()
{
    renderHeader();

    IMUDriver::IMUData& data = IMUDriver::GetData();

    switch (currentPage)
    {
        case Page::Accel:
        {
            Draw::Text(0, HEADER_HEIGHT + 4, "Accelerometer");
            {
                char str[16+1];
                sprintf(str, "X: %+1.2f G", data.accel_x_g);
                Draw::Text(0, HEADER_HEIGHT + 4 + 12, str);
            }
            {
                char str[16+1];
                sprintf(str, "Y: %+1.2f G", data.accel_y_g);
                Draw::Text(0, HEADER_HEIGHT + 4 + 12 * 2, str);
            }
            {
                char str[16+1];
                sprintf(str, "Z: %+1.2f G", data.accel_z_g);
                Draw::Text(0, HEADER_HEIGHT + 4 + 12 * 3, str);
            }
            break;
        }
        case Page::Gyro:
        {
            Draw::Text(0, HEADER_HEIGHT + 4, "Gyroscope");
            {
                char str[16+1];
                sprintf(str, "X: %+1.2f deg/s", data.gyro_x_ds);
                Draw::Text(0, HEADER_HEIGHT + 4 + 12, str);
            }
            {
                char str[16+1];
                sprintf(str, "Y: %+1.2f deg/s", data.gyro_y_ds);
                Draw::Text(0, HEADER_HEIGHT + 4 + 12 * 2, str);
            }
            {
                char str[16+1];
                sprintf(str, "Z: %+1.2f deg/s", data.gyro_z_ds);
                Draw::Text(0, HEADER_HEIGHT + 4 + 12 * 3, str);
            }
            break;
        }
        default:
            break;
    }
}

void MenuIMU::onUpdate()
{
    if (updateCounter++ >= SCREEN_REFRESH_RATE / updateFreq)
    {
        updateCounter = 0;
        triggerRender();
    }
}
