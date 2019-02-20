#include "StateTitle.h"



StateTitle::StateTitle()
{
	this->StateName = "Title";
}


StateTitle::~StateTitle()
{
}

void StateTitle::OnEnter()
{
}

void StateTitle::OnExit()
{
}

void StateTitle::OnUpdate(double dt)
{
	/*
	if ((Application::IsKeyPressed('5')) && *gameToggle == false && *bounceTime <= 0.0)
	{
		*gameToggle = true;
		*bounceTime = 0.3;
		this->spawnState = "Game";
		this->readyExitlocal = true;
	}
	*/

	if (Application::IsKeyPressed(VK_LBUTTON) && *bounceTime <= 0.0)
	{
		*bounceTime = 0.3;
		this->spawnState = "Menus";
		this->readyExitlocal = true;
	}
}

void StateTitle::OnRender()
{
	this->RenderTextScreen(this->meshGetFast("saofontsheet"), "SISASG", Color(255, 255, 255), 6.f, 5.5f, 1.f);
	this->RenderTextScreen(this->meshGetFast("saofontsheet"), "Is Seriously not A Space Game!", Color(255, 255, 255), 4.f, 4.5f, 2.5f);
}

void StateTitle::OnCam(int X, int Y, float XChange, float YChange)
{
}
