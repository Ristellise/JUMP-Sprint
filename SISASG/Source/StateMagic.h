#pragma once
#include "State.h"
class StateMagic :
    public GState
{
public:
    StateMagic();
    ~StateMagic();

    bool replaced = false;
    bool credits = false;

    std::string dtimestring;

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void Set2D();
    void Wrap2D();
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);
};

