#include "StateStat.h"

StateStat::StateStat()
{
	this->StateName = "Stat";
}

void StateStat::OnEnter()
{
	// money update
	this->STData->fileSaver.setValue("money", std::to_string(this->STData->moneyData));

	// Legacy code for abandoned feature - Do not open!
	/*
	// this->STData->fileSaver.setValue("level", std::to_string(this->STData->planetSelect + 1));
	*/

	// save
	this->STData->fileSaver.saveFile("config.ini");
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
	this->dtimestring += "\nPRESS 'R' TO RETURN TO MENUS";
	
	if (Application::IsKeyPressed('R'))
	{
		this->readyExitlocal = true;
		this->spawnState = "Menus";
	}
}

void StateStat::OnRender()
{
	this->RenderTextScreen(this->STData->font, this->dtimestring, Color(255, 255, 255), 4.f, 1.f, 9.5f);
}

void StateStat::OnCam(int X, int Y, float XChange, float YChange)
{
}

void StateStat::OnExit()
{
}