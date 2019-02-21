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
	if ((Application::IsKeyPressed('5')) && this->STData->gameToggle == false && this->STData->bounceTime <= 0.0)
	{
        this->STData->gameToggle = true;
        this->STData->bounceTime = 0.3;
		this->spawnState = "Game";
		this->readyExitlocal = true;
	}
	*/

	if (Application::IsKeyPressed(VK_LBUTTON))
	{
        this->STData->bounceTime = 0.3;
		this->spawnState = "Menus";
		this->readyExitlocal = true;
	}
}

void StateTitle::OnRender()
{
	this->RenderTextScreen(this->STData->font, "SISASG", Color(255, 255, 255), 6.f, 6.f, 1.f);
	this->RenderTextScreen(this->STData->font, "Is Seriously not A Space Game!", Color(255, 255, 255), 4.f, 6.5f, 2.5f);
}

void StateTitle::OnCam(int X, int Y, float XChange, float YChange)
{
}
