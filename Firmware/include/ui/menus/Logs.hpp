#pragma once
#include "ui/Menus.hpp"

class MenuLogs : public Menus::Menu
{
public:
    MenuLogs() = default;
    MenuLogs(Menu* parent);
    virtual ~MenuLogs() = default;

protected:
    virtual bool onBack() override;
    virtual bool onSelect() override;
    virtual bool onNext() override;
    virtual bool onPrev() override;

    virtual void onShow() override;
    virtual void onHide() override;
    virtual void onRender() override;
    virtual void onUpdate() override;

private:
    int updateCounter = 0;
    int updateFrequency = 2; // Hz
};