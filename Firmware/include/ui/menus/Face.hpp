#pragma once
#include "ui/Menus.hpp"
#include "ui/menus/Main.hpp"

struct FaceEyesInfo
{
    // Eyes look X direction, range [-1.0 (right), 1.0 (left)]
    float look_x;
    // Eyes look Y direction, range [-1.0 (down), 1.0 (up)]
    float look_y;
    // Skew of the eyes, range [-1.0 (left above right), 1.0 (right above left)]
    float skew;
    // Size multiplier for the eyes, 1.0 is default size
    float size;
    // Left eye open ratio, 1.0 is fully open, 0.0 is fully closed
    float open_left;
    // Right eye open ratio, 1.0 is fully open, 0.0 is fully closed
    float open_right;
    // Left eye top inner lid ratio, 1.0 is fully covering the eye, 0.0 is not covering at all 
    float lid_in_left;
    // Right eye top inner lid ratio, 1.0 is fully covering the eye, 0.0 is not covering at all
    float lid_in_right;
    // Left eye top outer lid ratio, 1.0 is fully covering the eye, 0.0 is not covering at all
    float lid_out_left;
    // Right eye top outer lid ratio, 1.0 is fully covering the eye, 0.0 is not covering at all
    float lid_out_right;
    // Left eye bottom lid ratio, 1.0 is fully covering the eye, 0.0 is not covering at all
    float lid_bottom_left;
    // Right eye bottom lid ratio, 1.0 is fully covering the eye, 0.0 is not covering at all
    float lid_bottom_right;
};

using BehaviorFunction = void(*)(FaceEyesInfo& eyes_info, uint32_t time_ms);

class MenuFace : public Menus::Menu
{
public:
    static MenuFace* GetInstance() { return instance; }

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
    static MenuFace* instance;

    BehaviorFunction m_behavior = nullptr;

    MenuMain menuMain;
};

void Behavior_Blink(FaceEyesInfo& eyes_info, uint32_t time_ms);
void Behavior_Idle(FaceEyesInfo& eyes_info, uint32_t time_ms);