#pragma once
#include <vector>

#include "Uniforms.h"
#include "State.h"
class StateManager
{
    std::vector<GState*> activeStates;
    std::vector<GState> availableStates;
    bool addState(std::string stateName);
    FontLoader* SM_FLInstance;
    unsigned *StateMan_parameters[]; // copy of m_parameters.

public:
    StateManager();
    void Update();
    void Render();
    bool Init();
    void addAvailable(GState state);
    
    ~StateManager();
};