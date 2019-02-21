#pragma once
#include <string>
#include "Mesh.h"
#include "Utility.h"
#include "FontLoader.h"
#include "Uniforms.h"
#include "MatrixStack.h"
#include "entity.h"
#include "Camera3.h"
#include "MouseHandler.h"
#include "collision.h"
#include <soloud.h>
#include <soloud_wavstream.h>
#include <soloud_wav.h>
// Contains a base template of game states Active.
// They may be added or removed.

struct StateData
{
    bool debugToggle = false;
    bool gameToggle = false;
    double bounceTime = 0.0;
    int shipSelect = 0;
    int planetSelect = 0;
    SoLoud::Soloud VERYLOUD;
    unsigned int hoopsMap;
	Mesh* font;
};

class GState
{
protected:
    bool mouseLock = false;
    bool readyExitlocal = false;
    std::string spawnState = "";
    FontLoader* St_FLInstance;
    unsigned *state_params;
	MS *modelStack, *viewStack, *projectionStack;
    Camera3* state_cam;
    MouseHandler* mouse;

    std::vector<entity*> *entitylists;
    std::vector<Mesh*> *meshList;

    collision* collideInstance;
public:
    StateData* STData;

    void Setlists(std::vector<entity*> *entitylists, std::vector<Mesh*> *meshList);

    GState();
    ~GState();
    Mesh * meshGetFast(std::string meshname);
    entity * entityGetFast(std::string meshname);
    void RenderText(Mesh * mesh, std::string text, Color color);
    void RenderMesh(Mesh * mesh, bool enableLight);
    std::string StateName = "NULL!!";
    std::string getspawnState() { return this->spawnState; };
    void resetspawnState() { this->spawnState = ""; };
    virtual void OnUpdate(double dt) = 0; // Logic Calls goes first
    virtual void OnExit() = 0; // Exit Calls after
    virtual void OnRender() = 0; // Rendering Calls=
    virtual void OnCam(int X, int Y, float XChange, float YChange) = 0;
    void OnCreate(
		unsigned * parameters, 
		FontLoader * St_FLInstance, 
		Camera3 * cam, 
		MouseHandler* mouse, 
		collision* collideInstance
	);
	void SetMatrixes(MS * model, MS * view, MS * projection);
    // GState Actually becomes active. does all the setup for the Uniforms and stuff.
    void RenderTextScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
    virtual void OnEnter() = 0; // GState initalisation
    bool readyExit() {return this->readyExitlocal; };
    void resetExit() {this->readyExitlocal = false; };
    bool toggleMouseLock() { if (this->mouseLock) { this->mouseLock = true; } else { this->mouseLock = false; }; };
};