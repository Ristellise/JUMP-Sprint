#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

// for debugging purposes only
#include <iostream>

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->position = defaultPosition = pos;
    this->target = defaultTarget = target;
    view = (target - position).Normalized();
    right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->up = defaultUp = right.Cross(view).Normalized();
    velocity = 0.f;
    camUpOffset = 4;
}

void Camera3::setWindow(WindowManager * winMan)
{
    this->winMan = winMan;
}

void Camera3::Update(double dt, entity targetShip)
{
    right = targetShip.right;
    up = targetShip.up;
    view = targetShip.view;

	// Legacy code for up/right/view vectors
	/*
	right = view.Cross(up).Normalized();
    up = right.Cross(view).Normalized();
    view = position - target;
	*/

	// Legacy code which produced unintended feature (camera snapping)
	/*
    if (targetShip.lKey == true)
    {
        Mtx44 rotation;
        rotation.SetToRotation(targetShip.angle, up.x, up.y, up.z);
        view = rotation * view;
    }

    if (targetShip.rKey == true)
    {
        Mtx44 rotation;
        rotation.SetToRotation(-targetShip.angle, up.x, up.y, up.z);
        view = rotation * view;
    }

    if (targetShip.uKey == true)
    {
        Mtx44 rotation;
        rotation.SetToRotation(targetShip.angle, right.x, right.y, right.z);
        view = rotation * view;
    }

    if (targetShip.dKey == true)
    {
        Mtx44 rotation;
        rotation.SetToRotation(-targetShip.angle, right.x, right.y, right.z);
        view = rotation * view;
    }

    if (targetShip.qKey == true)
    {
        Mtx44 rotation;
        rotation.SetToRotation(-targetShip.angle, view.x, view.y, view.z);
        view = rotation * view;
    }

    if (targetShip.eKey == true)
    {
        Mtx44 rotation;
        rotation.SetToRotation(targetShip.angle, view.x, view.y, view.z);
        view = rotation * view;
    }
	*/

	position = (-view * 40) + target;

    target.Set(
        targetShip.position.x + (up.x * camUpOffset),
        targetShip.position.y + (up.y * camUpOffset),
        targetShip.position.z + (up.z * camUpOffset)
    );
}

void Camera3::Updatealt(double dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);

    if (this->winMan->IsKeyPressed('N'))
    {
        position = position + up * (float)(40.f * dt);
        target = position + view;
    }

    if (this->winMan->IsKeyPressed('M'))
    {
        position = position - up * (float)(40.f * dt);
        target = position + view;
    }

    if (this->winMan->IsKeyPressed('R'))
    {
        Reset();
    }
}

void Camera3::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
    velocity = 0.f;
}