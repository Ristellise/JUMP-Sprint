#pragma once
#include "State.h"

class StateMenus : public GState
{
private:
    float rotateAngle;
    float movement_asteroid1_z;

public:
    StateMenus();
    ~StateMenus();
    std::string dtimestring;

    float size = 10.0f;
    float xpos = 10.0f;
    float ypos = 10.0f;

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);
};