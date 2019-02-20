#include "StateTitle.h"



StateTitle::StateTitle()
{
	this->StateName = "title";
}


StateTitle::~StateTitle()
{
}

void StateTitle::OnEnter()
{
	this->titleString1 = "             SISASG             ";
	this->titleString2 = "\nIs Seriously not A Space Game!";
}

void StateTitle::OnExit()
{
}

void StateTitle::OnUpdate(double dt)
{
	
	if ((Application::IsKeyPressed('5')) && *gameToggle == false && *bounceTime <= 0.0)
	{
		*gameToggle = true;
		*bounceTime = 0.3;
		this->spawnState = "Game";
		this->readyExitlocal = true;
	}
	
	if (Application::IsKeyPressed(VK_RETURN))
	{
		*bounceTime = 0.3;
		this->spawnState = "Menus";
		this->readyExitlocal = true;
	}
}

void StateTitle::OnRender()
{
	this->RenderTextScreen(this->meshGetFast("saofontsheet"), this->titleString1, Color(255, 255, 255), 6.f, -0.2f, 1.f);
	this->RenderTextScreen(this->meshGetFast("saofontsheet"), this->titleString2, Color(255, 255, 255), 4.f, 4.5f, 3.f);
}

void StateTitle::OnCam(int X, int Y, float XChange, float YChange)
{
}
