#include "ui/menus/WiFi.hpp"
#include "common/Log.hpp"
#include "ui/Icons.hpp"
#include "ui/Draw.hpp"
#include "Robot.hpp"

#include "qrcodegen.h"

MenuWiFi::MenuWiFi(Menu* parent)
    : Menu("WiFi", parent, Icons::WiFiMenu)
{
}

bool MenuWiFi::onBack()
{
    return false;
}

bool MenuWiFi::onSelect()
{
    return false;
}

bool MenuWiFi::onNext()
{
    qr_mode = !qr_mode;
    triggerRender();
    return true;
}

bool MenuWiFi::onPrev()
{
    qr_mode = !qr_mode;
    triggerRender();
    return true;
}


void MenuWiFi::onShow()
{
    qr_mode = false;
}

void MenuWiFi::onHide()
{ 
}

void MenuWiFi::onRender()
{
    WiFiManager& man = Robot::GetInstance().getNetworkManager().getWiFiManager();

    renderHeader();

    if (qr_mode)
    {
        uint8_t qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(4)];
        uint8_t tempBuffer[qrcodegen_BUFFER_LEN_FOR_VERSION(4)];
        int size;

        // If AP mode, generate Wi-Fi network QR Code
        if (man.getMode() == WiFiManager::Mode::AccessPoint)
        {
            char wifi_str[128];
            if (strlen(WIFI_AP_PASSWORD) > 0) // Password (WPA/WPA2)
            {
                snprintf(wifi_str, sizeof(wifi_str), "WIFI:S:%s;T:WPA;P:%s;;", WIFI_AP_SSID, WIFI_AP_PASSWORD);
            }
            else // No password (Open)
            {
                snprintf(wifi_str, sizeof(wifi_str), "WIFI:S:%s;T:nopass;;", WIFI_AP_SSID);
            }

            bool ok = qrcodegen_encodeText(wifi_str, tempBuffer, qrcode, qrcodegen_Ecc_LOW, qrcodegen_VERSION_MIN, 4, qrcodegen_Mask_AUTO, true);
            size = qrcodegen_getSize(qrcode);
            if (!ok) LOG_ERROR("MenuWiFi", "Couldn't generate QRCode for Wi-Fi STA");
        }
        else // If in STA mode, generate a QRCode to the web dashboard
        {
            char url_str[64];
            snprintf(url_str, sizeof(url_str), "http://%s", man.getIPAddr());

            bool ok = qrcodegen_encodeText(url_str, tempBuffer, qrcode, qrcodegen_Ecc_LOW, qrcodegen_VERSION_MIN, 4, qrcodegen_Mask_AUTO, true);
            size = qrcodegen_getSize(qrcode);
            if (!ok) LOG_ERROR("MenuWiFi", "Couldn't generate QRCode for URL");
        }

        constexpr const int QR_PADDING = 2;

        // Quick overflow check
        if (size*2 + QR_PADDING * 2 > ScreenDriver::info.height - HEADER_HEIGHT)
        {
            Draw::Text(0, HEADER_HEIGHT + 4, "Cannot display  QR Code.");
            LOG_ERROR("MenuWiFi", "Cannot display QR Code of size %d", size);
            return;
        }

        // display QR Code on the screen
        int corner_x = (ScreenDriver::info.width - size*2) / 2;
        int corner_y = (ScreenDriver::info.height - HEADER_HEIGHT - size*2) / 2 + HEADER_HEIGHT;

        Draw::RectRounded(corner_x - QR_PADDING, corner_y - QR_PADDING, size*2 + QR_PADDING * 2, size*2 + QR_PADDING * 2, 2);

        for (int j = 0; j < size; j++)
        {
            for (int i = 0; i < size; i++)
            {
                if (qrcodegen_getModule(qrcode, i, j)) 
                {
                    Draw::RectFilled(corner_x + i*2, corner_y + j*2, 2, 2, ScreenDriver::COLOR_BLACK);
                }
            }
        }
    }
    else
    {
        char mode_text[16];
        sprintf(mode_text, "Mode: %s", (man.getMode() == WiFiManager::Mode::AccessPoint)? "AP": "STA");
        uint16_t width = Draw::GetTextWidth(mode_text);
        Draw::Text<true>(ScreenDriver::info.width - width - 1, HEADER_HEIGHT + 4, mode_text);
        
        Draw::Text<true>(0, HEADER_HEIGHT + 04, "IP:");
        Draw::Text<true>(0, HEADER_HEIGHT + 14, (man.getIPAddr() != nullptr)? man.getIPAddr(): "---.---.---.---");
        
        Draw::Text<true>(0, HEADER_HEIGHT + 26, "SSID:");
        Draw::Text<true>(0, HEADER_HEIGHT + 36, (man.getSSID() != nullptr)? man.getSSID(): "---");
    }
}

void MenuWiFi::onUpdate()
{
    if (counter++ > SCREEN_REFRESH_RATE)
    {
        counter = 0;
        triggerRender();
    }
}
