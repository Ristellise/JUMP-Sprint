#include "StateManager.h"
#include "State.h"


StateManager::StateManager()
{
}

void StateManager::Update()
{
    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnUpdate();
        
        if (this->activeStates[i]->getspawnState() != nullptr)
        {
            this->addState(*(this->activeStates[i]->getspawnState()));
        }
        if (this->activeStates[i]->readyExit())
        {
            this->activeStates[i]->OnExit();
            delete this->activeStates[i];
            this->activeStates.erase(this->activeStates.begin() + i);
        }
    }
}
void StateManager::Render()
{
    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnRender();
    }
}
bool StateManager::Init()
{
    this->addState("init");
    return true;
}

bool StateManager::addState(std::string Statename)
{
    for (size_t i = 0; i < this->availableStates.size(); i++)
    {
        if (this->availableStates[i].StateName == Statename)
        {
            GState* newState;
            newState = &this->availableStates[i];
            newState->OnCreate(this->StateMan_parameters[0],this->SM_FLInstance);
            newState->OnEnter();
            this->activeStates.push_back(newState);
            return true;
        }
    }
    return false;
}


StateManager::~StateManager()
{
}
