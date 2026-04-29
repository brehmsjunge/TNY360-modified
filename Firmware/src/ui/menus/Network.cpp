#include "ui/menus/Network.hpp"
#include "ui/Icons.hpp"

MenuNetwork::MenuNetwork(Menu* parent)
    : MenuList("Network", parent, Icons::NetworkMenu),
    menuWiFi(MenuWiFi(this)), menuBluetooth(MenuBluetooth(this))
{
    setItems({
        &menuWiFi,
        &menuBluetooth
    });
}
