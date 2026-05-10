#pragma once
#include "ui/Menus.hpp"
#include "common/config.hpp"

class MenuMotorCalib : public Menus::Menu
{
public:
    MenuMotorCalib() = default;
    MenuMotorCalib(Menu* parent);
    virtual ~MenuMotorCalib() = default;

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
    int jointId;
    bool calibrating = false;

    int updateFreq = SCREEN_REFRESH_RATE / 5; // Update every 200ms
    int updateCounter = 0;
};