#pragma once
#include <vector>
#include "MouseHandler.h"
#include "Uniforms.h"
#include "State.h"
class StateManager
{
    std::vector<GState*> activeStates;
    std::vector<GState*> availableStates;
    bool addState(std::string stateName);
    FontLoader* SM_FLInstance;
    unsigned *StateMan_parameters; // copy of m_parameters.
    Mesh* StatemeshList[NUM_GEOMETRY]; // Temporary while we move to a vector solution.
    Camera3* manager_cam;
    MouseHandler* SM_Mouse;

    // Mouselatch
    bool lockswitch = true;
    bool currentlock = false;
    double lockbounce = 0.0;
	int fdelay;
public:
    bool setCam(Camera3* cam);
    StateManager();
    void Update(double dt, GLFWwindow * window);
    void Render();
    bool Init(unsigned * m_parameters, FontLoader * FLInstance, MouseHandler * SM_Mouse);
    bool InitmeshList(Mesh *StatemeshList);
    void addAvailable(GState* state);
	void SetMatrixes(MS * model, MS * view, MS * projection);
    bool Shutdown();
	MS *modelStack;
	MS *viewStack;
	MS *projectionStack;
    ~StateManager();
};