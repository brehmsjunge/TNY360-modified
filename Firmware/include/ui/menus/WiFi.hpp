#pragma once
#include "ui/Menus.hpp"

class MenuWiFi : public Menus::Menu
{
public:
    MenuWiFi() = default;
    MenuWiFi(Menu* parent);
    virtual ~MenuWiFi() = default;

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
    int counter = 0;
    bool qr_mode = false;
};