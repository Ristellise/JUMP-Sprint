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

void Camera3::Update(double dt, entity targetShip)
{
	right = targetShip.right;
	up = targetShip.up;
	view = targetShip.view;
	// right = view.Cross(up).Normalized();
	// up = right.Cross(view).Normalized();
	// view = position - target;

	if (Application::IsKeyPressed(VK_LEFT))
	{
		Mtx44 rotation;
		rotation.SetToRotation(targetShip.angle, up.x, up.y, up.z);
		view = rotation * view;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		Mtx44 rotation;
		rotation.SetToRotation(targetShip.angle, up.x, up.y, up.z);
		view = rotation * view;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		Mtx44 rotation;
		rotation.SetToRotation(-targetShip.angle, right.x, right.y, right.z);
		view = rotation * view;
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		Mtx44 rotation;
		rotation.SetToRotation(-targetShip.angle, right.x, right.y, right.z);
		view = rotation * view;
	}

	if (Application::IsKeyPressed('Q'))
	{
		Mtx44 rotation;
		rotation.SetToRotation(-targetShip.angle, view.x, view.y, view.z);
		view = rotation * view;
	}

	if (Application::IsKeyPressed('E'))
	{
		Mtx44 rotation;
		rotation.SetToRotation(-targetShip.angle, view.x, view.y, view.z);
		view = rotation * view;
	}

	position = (-view * 40) + target;

	target.Set(
		targetShip.position.x + (up.x * camUpOffset),
		targetShip.position.y + (up.y * camUpOffset),
		targetShip.position.z + (up.z * camUpOffset)
	);

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera3::Updatealt(double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	if (Application::IsKeyPressed('N'))
	{
		position = position + up * (float)(40.f * dt);
		target = position + view;
	}

	if (Application::IsKeyPressed('M'))
	{
		position = position - up * (float)(40.f * dt);
		target = position + view;
	}

	if (Application::IsKeyPressed('R'))
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