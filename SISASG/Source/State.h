#pragma once
#include <string>
#include "Mesh.h"
#include "Utility.h"
#include "GL\glew.h"
#include "FontLoader.h"
#include "Uniforms.h"
#include "MatrixStack.h"
#include "entity.h"
#include "Camera3.h"
#include "MouseHandler.h"
// Contains a base template of game states Active.
// They may be added or removed.
class GState
{
protected:
    bool mouseLock = false;
    bool readyExitlocal = false;
    std::string* spawnState = nullptr;
    FontLoader* St_FLInstance;
    unsigned *state_params;
    std::vector<entity*> entitylists;
    std::vector<Mesh*> meshList;
    MS *modelStack, *viewStack, *projectionStack;
    Camera3* state_cam;
    MouseHandler* mouse;
    // TODO: Vector of entities
public:
    GState();
    ~GState();
    Mesh * meshGetFast(std::string meshname);
    entity * entityGetFast(std::string meshname);
    void RenderText(Mesh * mesh, std::string text, Color color);
    void RenderMesh(Mesh * mesh, bool enableLight);
    std::string StateName = "NULL!!";
    std::string* getspawnState() { return this->spawnState; };
    virtual void OnUpdate(double dt) = 0; // Logic Calls goes first
    virtual void OnExit() = 0; // Exit Calls after
    virtual void OnRender() = 0; // Rendering Calls=
    virtual void OnCam(int X, int Y, float XChange, float YChange) = 0; // Rendering Calls=
    void OnCreate(unsigned * parameters, FontLoader * St_FLInstance, Camera3 * cam, MouseHandler * mouse);
    // GState Actually becomes active. does all the setup for the Uniforms and stuff.
    void RenderTextScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
    virtual void OnEnter() = 0; // GState initalisation
    bool readyExit() {return this->readyExitlocal; };
    void toggleMouseLock() { if (this->mouseLock) { this->mouseLock = true; } else { this->mouseLock = false; }; };
    bool mouseLocked() { return this->mouseLock; };
};