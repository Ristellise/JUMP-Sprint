#include "StateManager.h"
#include "State.h"
#include "Application.h"

bool StateManager::setCam(Camera3 * cam)
{
    this->manager_cam = cam;
    return true;
}

StateManager::StateManager()
{
}

void StateManager::Update(double dt, GLFWwindow *window)
{
    bool last_lock = this->currentlock;
    this->SM_Mouse->Update(window, dt);
    this->lockswitch = true;

    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnCam(this->SM_Mouse->X, this->SM_Mouse->Y, this->SM_Mouse->XChange, this->SM_Mouse->YChange);
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
        if (!this->activeStates[i]->mouseLocked())
        {
            this->lockswitch = false;
        }
    }
    if (Application::IsKeyPressed('P') && fdelay == 0)
    {
        this->lockswitch = true;
		fdelay += 10;
    }
	if (fdelay > 0)
	{
		fdelay--;
	}
    if (this->currentlock != this->lockswitch && this->lockbounce > 1.0)
    {
        this->lockbounce = 0.0;
        this->currentlock = this->lockswitch;
    }
    else
    {
        this->lockbounce += dt;
    }
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL && this->lockswitch) // if cursor is on and needs locking
    {
        std::cout << "Mouse Off" << std::endl;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        this->lockswitch = true;
        this->currentlock = false;
    }
    else if (this->currentlock && this->lockswitch) // if cursor is currently locked
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::cout << "Mouse On" << std::endl;
        this->lockswitch = false;
    }
}
void StateManager::Render()
{
    for (size_t i = 0; i < this->activeStates.size(); i++)
    {
        this->activeStates[i]->OnRender();
    }
}
bool StateManager::Init(unsigned *m_parameters, FontLoader* FLInstance,MouseHandler* SM_Mouse)
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

bool StateManager::addState(std::string Statename)
{
    for (size_t i = 0; i < this->availableStates.size(); i++)
    {
        if ((*this->availableStates[i]).StateName == Statename)
        {
            (this->availableStates[i])->OnCreate(StateMan_parameters,this->SM_FLInstance,this->manager_cam, this->SM_Mouse);
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
