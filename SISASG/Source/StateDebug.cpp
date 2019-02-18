#include "StateDebug.h"
/*-----
StateDebug:
- Used for All Debug overlays that are used.
- Write them here.
*/


StateDebug::StateDebug()
{
}

void StateDebug::OnEnter()
{
}

void StateDebug::OnExit()
{
}

void StateDebug::OnUpdate(double dt)
{
    entity testCube1 = *this->entityGetFast("spaceship");
    this->dtimestring = "FPS:";
    this->dtimestring += std::to_string(1.0f / dt);
    this->dtimestring += "\nCam X:";
    this->dtimestring += std::to_string(this->state_cam->position.x);
    this->dtimestring += "\nCam Y:";
    this->dtimestring += std::to_string(this->state_cam->position.y);
    this->dtimestring += "\nCam Z:";
    this->dtimestring += std::to_string(this->state_cam->position.z);
    this->dtimestring += "\nVel :";
    this->dtimestring += std::to_string(testCube1.velocity);
    this->dtimestring += "\nAcl :";
    this->dtimestring += std::to_string(testCube1.accl);
    this->dtimestring += "\nPit :";
    this->dtimestring += std::to_string(testCube1.pitchTotal);
    this->dtimestring += "\nYaw :";
    this->dtimestring += std::to_string(testCube1.yawTotal);
    this->dtimestring += "\nRoll :";
    this->dtimestring += std::to_string(testCube1.rollTotal);
}

void StateDebug::OnRender()
{
}

void StateDebug::OnCam(int X, int Y, float XChange, float YChange)
{
}

StateDebug::~StateDebug()
{
}
