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
    double bounceTime = 0.0;
	bool debugToggle = false;
	bool gameToggle = false;
	int fdelay;

    collision collideInstance;
public:
	int shipSelect;
	// 0 - Starter ship
	// 1 - Heavy ship
	// 2 - Speed ship

	int planetSelect;
	// 0 - Earth
	// 1 - Venus
	// 2 - Mars
	// 3 - Jupiter

	std::vector<entity*> entitylists;
	std::vector<Mesh*> meshList;

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