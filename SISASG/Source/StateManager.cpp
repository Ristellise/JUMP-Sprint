#include "StateManager.h"
#include "State.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
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
    this->StateManagerData.VERYLOUD.init();
	Mesh* meshbuffer;
	meshbuffer = MeshBuilder::GenerateText("saofontsheet", *FLInstance);
	meshbuffer->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);
	this->StateManagerData.font = meshbuffer;
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
			this->manager_cam->Reset();
            (this->availableStates[i])->OnCreate(
				StateMan_parameters, 
				this->SM_FLInstance, 
				this->manager_cam,
				this->SM_Mouse, 
				&this->collideInstance
			);

			(this->availableStates[i])->STData = &this->StateManagerData;
            (this->availableStates[i])->Setlists(&this->entitylists, &this->meshList);
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
