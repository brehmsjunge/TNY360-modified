#pragma once
#include "ui/Menus.hpp"
#include "ui/menus/List.hpp"
#include "ui/menus/WiFi.hpp"
#include "ui/menus/Bluetooth.hpp"

class MenuNetwork : public MenuList
{
public:
    MenuNetwork() = default;
    MenuNetwork(Menu* parent);
    virtual ~MenuNetwork() = default;

private:
    MenuWiFi menuWiFi;
    MenuBluetooth menuBluetooth;
};