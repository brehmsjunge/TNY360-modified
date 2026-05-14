#pragma once
#include "ui/Menus.hpp"

class MenuReset : public Menus::Menu
{
public:
    MenuReset() = default;
    MenuReset(Menu* parent);
    virtual ~MenuReset() = default;

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
    bool isResetting = false;
};