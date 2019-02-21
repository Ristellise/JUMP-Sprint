#include "StateShop.h"

StateShop::StateShop()
{
	this->StateName = "Shop";
}

void StateShop::OnEnter()
{
    this->STData->SoundSrcs.insert_or_assign("testTrack", new SoundContainer(&this->STData->VERYLOUD, "Audio/xmtest.xm", SourceType::ST_OPENMPT));
    this->STData->SoundSrcs["testTrack"]->play(true);
}

void StateShop::OnExit()
{
}

void StateShop::OnUpdate(double dt)
{
}

void StateShop::OnRender()
{
}

void StateShop::OnCam(int X, int Y, float XChange, float YChange)
{
}

StateShop::~StateShop()
{
}
