#pragma once
#include "ui/Menus.hpp"

class MenuBluetooth : public Menus::Menu
{
public:
    MenuBluetooth() = default;
    MenuBluetooth(Menu* parent);
    virtual ~MenuBluetooth() = default;

protected:
    virtual bool onBack() override;
    virtual bool onSelect() override;
    virtual bool onNext() override;
    virtual bool onPrev() override;

    virtual void onShow() override;
    virtual void onHide() override;
    virtual void onRender() override;
    virtual void onUpdate() override;
};