#pragma once
#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "spaceship.h"

#include "MouseHandler.h"
#include "Uniforms.h"
#include "StateManager.h"
#include <math.h>
#include "State.h"

class StateShop : public GState
{
public:
    StateShop();
    ~StateShop();

    float rotateAngle;
    int NumberOfShips = 3;

    float Shift = 0;
    bool shiftmovement = false;
    int Dir = 0; // x Direction on UI
    float Delay = 0; // Frame delay

    bool lockUnlock;
    unsigned long long costOfShip;

    void RenderShips();
    void RenderUI();

    void OnEnter();
    void OnExit();
    void OnUpdate(double dt);
    void OnRender();
    void OnCam(int X, int Y, float XChange, float YChange);
};