#pragma once
#include "ui/Menus.hpp"
#include <vector>

class MenuList : public Menus::Menu
{
public:
    MenuList();
    MenuList(const char* title);
    MenuList(const char* title, Menu* parent);
    MenuList(const char* title, Menu* parent, const uint8_t icon[8]);
    MenuList(const char* title, Menu* parent, const uint8_t icon[8], std::vector<Menu*> items);
    virtual ~MenuList() = default;

    void setItems(std::vector<Menu*> items);

protected:
    uint8_t m_selected_index = 0;
    uint8_t m_selected_shift = 4;

    float m_view_shift_current = 0.0f;
    float m_selected_index_current = 0.0f;

    std::vector<Menu*> m_items;

    virtual bool onBack() override;
    virtual bool onSelect() override;
    virtual bool onNext() override;
    virtual bool onPrev() override;

    virtual void onShow() override;
    virtual void onHide() override;
    virtual void onRender() override;
    virtual void onUpdate() override;
};