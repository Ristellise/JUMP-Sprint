#pragma once
#include <string>
#include "Mesh.h"
#include "Utility.h"
#include "GL\glew.h"
#include "FontLoader.h"
#include "Uniforms.h"
#include "MatrixStack.h"
#include "entity.h"
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
    MS *modelStack, *viewStack, *projectionStack;
    // TODO: Vector of entities
public:
    GState();
    ~GState();
    void RenderText(Mesh * mesh, std::string text, Color color);
    void RenderMesh(Mesh * mesh, bool enableLight);
    std::string StateName = "NULL!!";
    std::string* getspawnState() { return this->spawnState; };
    void OnUpdate(); // Logic Calls goes first
    void OnExit(); // Exit Calls after
    void OnRender(); // Rendering Calls=
    void OnCreate(unsigned * parameters, FontLoader * St_FLInstance); // GState Actually becomes active. does all the setup for the Uniforms and stuff.
    void OnEnter(); // GState initalisation
    bool readyExit() {return this->readyExitlocal; };
    bool toggleMouseLock() { if (this->mouseLock) { this->mouseLock = true; } else { this->mouseLock = false; }; };
};