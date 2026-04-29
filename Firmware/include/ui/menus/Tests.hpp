#pragma once
#include "ui/Menus.hpp"
#include "ui/menus/List.hpp"
#include "ui/menus/I2C.hpp"

class MenuTests : public MenuList
{
public:
    MenuTests() = default;
    MenuTests(Menu* parent);
    virtual ~MenuTests() = default;

private:
    MenuI2C menuI2C;
};