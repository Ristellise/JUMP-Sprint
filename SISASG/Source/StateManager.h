#pragma once
#include <vector>

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
public:
    bool setCam(Camera3* cam);
    StateManager();
    void Update(double dt);
    void Render();
    bool Init(unsigned *m_parameters, FontLoader* FLInstance);
    bool InitmeshList(Mesh *StatemeshList);
    void addAvailable(GState* state);
    bool Shutdown();
    ~StateManager();
};