#include "StateManager.h"
#include "State.h"


bool StateManager::setCam(Camera3 * cam)
{
    this->manager_cam = cam;
    return true;
}

StateManager::StateManager()
{
}

void StateManager::Update(double dt)
{
    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnUpdate(dt);
        
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
bool StateManager::Init(unsigned *m_parameters, FontLoader* FLInstance)
{
    this->StateMan_parameters = m_parameters;
    this->SM_FLInstance = FLInstance;
    this->addState("init");
    return true;
}

bool StateManager::InitmeshList(Mesh * StatemeshList)
{
    *this->StatemeshList = StatemeshList;
    return true;
}

void StateManager::addAvailable(GState * state)
{
    this->availableStates.push_back(state);
}

bool StateManager::addState(std::string Statename)
{
    for (size_t i = 0; i < this->availableStates.size(); i++)
    {
        if ((*this->availableStates[i]).StateName == Statename)
        {
            (this->availableStates[i])->OnCreate(StateMan_parameters,this->SM_FLInstance,this->manager_cam);
            (this->availableStates[i])->OnEnter();
            this->activeStates.push_back(this->availableStates[i]);
            return true;
        }
    }
    return false;
}


bool StateManager::Shutdown()
{
    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnExit();
    }
    return true;
}

StateManager::~StateManager()
{
}
