#include "ui/menus/Reset.hpp"
#include "ui/Icons.hpp"
#include "ui/Draw.hpp"
#include "nvs_flash.h"
#include "esp_system.h"

MenuReset::MenuReset(Menu* parent)
    : Menu("Reset", parent, Icons::ResetMenu)
{
}

bool MenuReset::onBack()
{
    return false;
}

bool MenuReset::onSelect()
{
    isResetting = true;
    return true;
}

bool MenuReset::onNext()
{
    return false;
}

bool MenuReset::onPrev()
{
    return false;
}


void MenuReset::onShow()
{
    
}

void MenuReset::onHide()
{ 
}

void MenuReset::onRender()
{   
    if (isResetting)
    {
        const char* text = "Resetting ...";
        uint16_t width = Draw::GetTextWidth(text);
        Draw::Text(ScreenDriver::info.width / 2 - width / 2, HEADER_HEIGHT + 4, text);
        ScreenDriver::Upload(); // send data now

        // reset NVS and restart robot here
        // (not in update, so that the screen displayes the "resetting" message)
        nvs_flash_erase();
        esp_restart();
    }
    else
    {
        renderHeader();
        {
            const char* text = "Are you sure?";
            uint16_t width = Draw::GetTextWidth(text);
            Draw::Text(ScreenDriver::info.width / 2 - width / 2, HEADER_HEIGHT + 4, text);
        }
        {
            const char* text = "Back";
            Draw::Text(12, ScreenDriver::info.height - 9, text);
        }
        {
            const char* text = "Reset";
            uint16_t width = Draw::GetTextWidth(text);
            Draw::Text(ScreenDriver::info.width - width - 12, ScreenDriver::info.height - 9, text);
        }
        Draw::Blit(0, ScreenDriver::info.height - 9, 8, 8, (uint8_t*)Icons::ChevronLeft);
        Draw::Blit(ScreenDriver::info.width - 9, ScreenDriver::info.height - 9, 8, 8, (uint8_t*)Icons::ChevronRight);
    }
}

void MenuReset::onUpdate()
{
    triggerRender();
}
