#pragma once
#include "ui/Menus.hpp"
#include "ui/menus/Main.hpp"

struct FaceEyesInfo
{
    float look_x;
    float look_y;
    float skew;
    float size;
    float open_left;
    float open_right;
    float lid_in_left;
    float lid_in_right;
    float lid_out_left;
    float lid_out_right;
    float lid_bottom_left;
    float lid_bottom_right;
};

using BehaviorFunction = void(*)(FaceEyesInfo& eyes_info, uint32_t time_ms);

class MenuFace : public Menus::Menu
{
public:
    FaceEyesInfo eyes_info;

    MenuFace();
    virtual ~MenuFace();

    void setBehavior(BehaviorFunction behavior);

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
    BehaviorFunction m_behavior = nullptr;

    MenuMain menuMain;
};

void Behavior_Idle(FaceEyesInfo& eyes_info, uint32_t time_ms);