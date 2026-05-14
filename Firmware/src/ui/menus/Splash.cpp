#include "ui/menus/Splash.hpp"
#include "ui/Icons.hpp"
#include "ui/Draw.hpp"

bool MenuSplash::onBack()
{
    return false;
}

bool MenuSplash::onSelect()
{
    return false;
}

bool MenuSplash::onNext()
{
    return false;
}

bool MenuSplash::onPrev()
{
    return false;
}


void MenuSplash::onShow()
{
    triggerRender();
}

void MenuSplash::onHide()
{ 
}

void MenuSplash::onRender()
{
    Draw::Blit(32, 0, 64, 64, (uint8_t*) Icons::SplashScreen);
}

void MenuSplash::onUpdate()
{
}
