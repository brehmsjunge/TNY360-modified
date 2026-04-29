#include "ui/menus/Info.hpp"
#include "ui/Icons.hpp"

MenuInfo::MenuInfo(Menu* parent)
    : Menu("Info", parent, Icons::InfoMenu)
{
}

bool MenuInfo::onBack()
{
    return false;
}

bool MenuInfo::onSelect()
{
    return false;
}

bool MenuInfo::onNext()
{
    return false;
}

bool MenuInfo::onPrev()
{
    return false;
}


void MenuInfo::onShow()
{
}

void MenuInfo::onHide()
{ 
}

void MenuInfo::onRender()
{
    renderHeader();
}

void MenuInfo::onUpdate()
{
}
