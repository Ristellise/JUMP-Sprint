#pragma once
#include "State.h"
class StateWorld :
    public GState
{
public:
    StateWorld();
    ~StateWorld();
    std::string dtimestring;
    std::string StateName = "World";

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);
};

