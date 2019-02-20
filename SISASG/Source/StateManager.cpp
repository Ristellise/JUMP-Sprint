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

void StateManager::Update(double dt, GLFWwindow* window)
{
    this->SM_Mouse->Update(window,dt);
    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnUpdate(dt);
        this->activeStates[i]->OnCam(this->SM_Mouse->X, this->SM_Mouse->Y,
                                     this->SM_Mouse->XChange, this->SM_Mouse->YChange);
        if (this->activeStates[i]->readyExit())
        {
            this->activeStates[i]->OnExit();
            this->activeStates[i]->resetExit();
            
            
            for (std::vector< entity* >::iterator it = this->entitylists.begin(); it != this->entitylists.end(); ++it)
            {
                delete (*it);
            }
            this->entitylists.clear();
            this->entitylists.shrink_to_fit();
            if (this->activeStates[i]->getspawnState() != "")
            {
                this->addState((this->activeStates[i]->getspawnState()));
                this->activeStates[i]->resetspawnState();
            }
            this->activeStates.erase(this->activeStates.begin() + i);
        }
        else if (this->activeStates[i]->getspawnState() != "")
        {
            this->addState((this->activeStates[i]->getspawnState()));
			this->activeStates[i]->resetspawnState();
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
bool StateManager::Init(unsigned * m_parameters, FontLoader * FLInstance, MouseHandler * SM_Mouse)
{
    this->StateMan_parameters = m_parameters;
    this->SM_FLInstance = FLInstance;
    this->SM_Mouse = SM_Mouse;
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
void StateManager::SetMatrixes(MS* model, MS* view, MS* projection)
{
	this->modelStack = model;
	this->viewStack = view;
	this->projectionStack = projection;
}
bool StateManager::addState(std::string Statename)
{
    for (size_t i = 0; i < this->availableStates.size(); i++)
    {
        if ((*this->availableStates[i]).StateName == Statename)
        {
            (this->availableStates[i])->OnCreate(
				StateMan_parameters, 
				this->SM_FLInstance, 
				this->manager_cam,
				this->SM_Mouse, 
				&this->collideInstance, 
				&this->entitylists, 
				&this->meshList
			);
			(this->availableStates[i])->debugToggle = &this->debugToggle;
			(this->availableStates[i])->gameToggle = &this->gameToggle;
			(this->availableStates[i])->bounceTime = &this->bounceTime;
			(this->availableStates[i])->shipSelect = &this->shipSelect;
			(this->availableStates[i])->planetSelect = &this->planetSelect;
            (this->availableStates[i])->SetMatrixes(this->modelStack,this->viewStack,this->projectionStack);
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
