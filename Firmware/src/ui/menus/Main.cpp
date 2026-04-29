#include "ui/menus/Main.hpp"
#include "ui/Icons.hpp"

MenuMain::MenuMain(Menu* parent)
    : MenuList("Main menu", parent, Icons::MainMenu),
    menuNetwork(MenuNetwork(this)), menuTests(MenuTests(this)), menuSystem(MenuSystem(this))
{
    setItems({
        &menuNetwork,
        &menuTests,
        &menuSystem
    });
}
