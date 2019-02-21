#include "StateMenus.h"

StateMenus::StateMenus()
{
	this->StateName = "Menus";
}

void StateMenus::OnEnter()
{
    this->STData->SoundSrcs.insert_or_assign("testTrack", new SoundContainer(&this->STData->VERYLOUD, "Audio/xmtest.xm", SourceType::ST_OPENMPT));
    this->STData->SoundSrcs.insert_or_assign("looptest", new SoundContainer(&this->STData->VERYLOUD, "Audio/testloop.wav", SourceType::ST_NORMAL));
    this->STData->SoundSrcs["testTrack"]->play(true, true);
    this->STData->SoundSrcs["looptest"]->play(true, true);
    this->STData->SoundSrcs["looptest"]->loopPos(10.956f);
}

void StateMenus::OnExit()
{
    this->STData->SoundSrcs["testTrack"]->DIE(false);
}

void StateMenus::OnUpdate(double dt)
{
}

void StateMenus::OnRender()
{
	// Ship Hangar button
	if ((this->mouse->X > 100) && (this->mouse->X < 330) && (this->mouse->Y >380) && (this->mouse->Y < 450))
	{
		this->RenderTextScreen(this->STData->font, " > Start Game", Color(255, 255, 255), 6.f, 1.f, 0.f);

		if (Application::IsKeyPressed(VK_LBUTTON) && this->STData->bounceTime <= 0.0)
		{
			this->STData->bounceTime = 0.3;
			this->readyExitlocal = true;
			this->spawnState = "Hangar";
		}
	}
	else
	{
		this->RenderTextScreen(this->STData->font, "   Start Game", Color(255, 255, 255), 6.f, 1.f, 0.f);
	}

	// Ship Foundry button
	if ((this->mouse->X > 100) && (this->mouse->X < 210) && (this->mouse->Y > 450) && (this->mouse->Y < 520))
	{
		this->RenderTextScreen(this->STData->font, " > Shop", Color(255, 255, 255), 6.f, 1.f, -1.f);

		if (Application::IsKeyPressed(VK_LBUTTON) && this->STData->bounceTime <= 0.0)
		{
			this->STData->bounceTime = 0.3;
			this->readyExitlocal = true;
			this->spawnState = "Shop";
		}
	}
	else
	{
		this->RenderTextScreen(this->STData->font, "   Shop", Color(255, 255, 255), 6.f, 1.f, -1.f);
	}
}

void StateMenus::OnCam(int X, int Y, float XChange, float YChange)
{
}

StateMenus::~StateMenus()
{
}
