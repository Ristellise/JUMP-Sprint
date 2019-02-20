#include "StateDebug.h"
/*-----
StateDebug:
- Used for All Debug overlays that are used.
- Write them here.
*/


StateDebug::StateDebug()
{
	this->StateName = "debugger";
}

void StateDebug::OnEnter()
{
}

void StateDebug::OnExit()
{
}

void StateDebug::OnUpdate(double dt)
{
    // entity testCube1 = *this->entityGetFast("testcube");

	this->dtimestring = "FPS:";
	this->dtimestring += std::to_string(1.0f / dt);
	this->dtimestring += "\nCam X:";
	this->dtimestring += std::to_string(this->state_cam->position.x);
	this->dtimestring += "\nCam Y:";
	this->dtimestring += std::to_string(this->state_cam->position.y);
	this->dtimestring += "\nCam Z:";
	this->dtimestring += std::to_string(this->state_cam->position.z);

	/*
	this->dtimestring += "\nVel :";
	this->dtimestring += std::to_string(testCube1.velocity);
	this->dtimestring += "\nAcl :";
	this->dtimestring += std::to_string(testCube1.accl);
	this->dtimestring += "\nPit :";
	this->dtimestring += std::to_string(testCube1.pitchTotal);
	this->dtimestring += "\nYaw :";
	this->dtimestring += std::to_string(testCube1.yawTotal);
	this->dtimestring += "\nRol :";
	this->dtimestring += std::to_string(testCube1.rollTotal);
	this->dtimestring += "\nEntities With physics: " + std::to_string(this->collideInstance->updatingEnts);
	*/

	this->dtimestring += "\nMouse:" + std::to_string(this->mouse->X) +
		" | " + std::to_string(this->mouse->Y) +
		" | Change: " + std::to_string(this->mouse->XChange) +
		" | " + std::to_string(this->mouse->YChange);

	if ((Application::IsKeyPressed('6')) && this->STData->debugToggle == true && this->STData->bounceTime <= 0.0)
	{
        this->STData->debugToggle = false;
        this->STData->bounceTime = 0.3;
		this->readyExitlocal = true;
	}
}

void StateDebug::OnRender()
{
	this->RenderTextScreen(this->STData->font, this->dtimestring, Color(255, 255, 255), 2.f, 1.f, 24.f);
}

void StateDebug::OnCam(int X, int Y, float XChange, float YChange)
{
}

StateDebug::~StateDebug()
{
}
