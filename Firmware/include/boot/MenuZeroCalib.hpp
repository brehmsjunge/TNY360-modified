#pragma once
#include "ui/Menus.hpp"

class MenuZeroCalib : public Menus::Menu
{
public:
    constexpr static const char* TAG = "MenuZeroCalib";

    MenuZeroCalib() = default;
    virtual ~MenuZeroCalib() = default;

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
    bool calibrating = false;
    bool calibrated = false;
    int motorIndex = 0;
    int counter = 0;
};