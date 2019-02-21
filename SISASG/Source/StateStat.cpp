#include "StateStat.h"

StateStat::StateStat()
{
	this->StateName = "Stat";
}

void StateStat::OnEnter()
{
	this->spawnState = "Stat";
}

void StateStat::OnUpdate(double dt)
{
	this->dtimestring = "POINTS OBTAINED: ";
	this->dtimestring += std::to_string(this->STData->pointsPrev);
	this->dtimestring += "\nTIME REMAINING: ";
	this->dtimestring += std::to_string(this->STData->timePrev);
	this->dtimestring += "\nMONEY EARNED: ";
	this->dtimestring += std::to_string(this->STData->moneyEarned);
	this->dtimestring += "\nMONEY TOTAL: ";
	this->dtimestring += std::to_string(this->STData->moneyData);
}

void StateStat::OnRender()
{
	this->RenderTextScreen(this->STData->font, this->dtimestring, Color(255, 255, 255), 2.f, 4.f, 24.f);
}

void StateStat::OnCam(int X, int Y, float XChange, float YChange)
{
}

void StateStat::OnExit()
{
}