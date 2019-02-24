#pragma once
#include "State.h"

class StatePlanet : public GState
{
public:
    StatePlanet();
    ~StatePlanet();

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);
};