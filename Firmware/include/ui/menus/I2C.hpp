#pragma once
#include "ui/Menus.hpp"

class MenuI2C : public Menus::Menu
{
public:
    MenuI2C() = default;
    MenuI2C(Menu* parent);
    virtual ~MenuI2C() = default;

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
    uint8_t m_i2c_bus_select = 1; // 0 = secondary, 1 = primary
    uint8_t m_devices_list[64] = {0};
    uint8_t m_device_count = 0;
    uint8_t m_addr_scan = 0;
    bool m_scanning = false;

    void startScan();
};