#pragma once
#include "ui/Menus.hpp"

class MenuInfo : public Menus::Menu
{
public:
    MenuInfo() = default;
    MenuInfo(Menu* parent);
    virtual ~MenuInfo() = default;

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