#include "StateStat.h"

StateStat::StateStat()
{
	this->StateName = "Stat";
}

void StateStat::OnEnter()
{
	this->spawnState = "Stat";
}

void StateStat::OnRender()
{

}

void StateStat::OnCam(int X, int Y, float XChange, float YChange)
{
}

void StateStat::OnExit()
{
}

void StateStat::OnUpdate(double dt)
{
}