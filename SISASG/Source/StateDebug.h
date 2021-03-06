#pragma once
#include "State.h"
class StateDebug :
    public GState
{
public:
    StateDebug();
    ~StateDebug();
    std::string dtimestring;

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);
};

