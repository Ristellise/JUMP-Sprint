#include "StateMenus.h"

StateMenus::StateMenus()
{
	this->StateName = "Menus";
}

void StateMenus::OnEnter()
{
}

void StateMenus::OnExit()
{
}

void StateMenus::OnUpdate(double dt)
{
}

void StateMenus::OnRender()
{
	// Ship Hangar button
	if ((this->mouse->X > 150) && (this->mouse->X < 450) && (this->mouse->Y >380) && (this->mouse->Y < 450))
	{
		this->RenderTextScreen(this->meshGetFast("saofontsheet"), " > Start Game", Color(255, 255, 255), 6.f, 1.f, 0.f);

		if (Application::IsKeyPressed(VK_LBUTTON) && *bounceTime <= 0.0)
		{
			*bounceTime = 0.3;
			this->readyExitlocal = true;
			this->spawnState = "Hangar";
		}
	}
	else
	{
		this->RenderTextScreen(this->meshGetFast("saofontsheet"), "   Start Game", Color(255, 255, 255), 6.f, 1.f, 0.f);
	}

	// Ship Foundry button
	if ((this->mouse->X > 150) && (this->mouse->X < 450) && (this->mouse->Y > 450) && (this->mouse->Y < 520))
	{
		this->RenderTextScreen(this->meshGetFast("saofontsheet"), " > Shop", Color(255, 255, 255), 6.f, 1.f, -1.f);

		if (Application::IsKeyPressed(VK_LBUTTON) && *bounceTime <= 0.0)
		{
			*bounceTime = 0.3;
			this->readyExitlocal = true;
			this->spawnState = "Shop";
		}
	}
	else
	{
		this->RenderTextScreen(this->meshGetFast("saofontsheet"), "   Shop", Color(255, 255, 255), 6.f, 1.f, -1.f);
	}
}

void StateMenus::OnCam(int X, int Y, float XChange, float YChange)
{
}

StateMenus::~StateMenus()
{
}
