#include "ui/menus/Tests.hpp"
#include "ui/Icons.hpp"

MenuTests::MenuTests(Menu* parent)
    : MenuList("Tests", parent, Icons::TestsMenu),
    menuI2C(MenuI2C(this))
{
    setItems({
        &menuI2C
    });
}
