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

void Camera3::Update(
	double dt, 
	float targetx, 
	float targety, 
	float targetz,
	float targetAngle,
	Vector3 targetRight,
	Vector3 targetUp,
	Vector3 targetView
)
{
	right = targetRight;
	up = targetUp;
	view = targetView;
	// right = view.Cross(up).Normalized();
	// up = right.Cross(view).Normalized();
	// view = position - target;

	if ((Application::IsKeyPressed(VK_LEFT)) || (Application::IsKeyPressed(VK_RIGHT)))
	{
		Mtx44 rotation1;
		rotation1.SetToRotation(targetAngle, up.x, up.y, up.z);
		view = rotation1 * view;
	}

	if ((Application::IsKeyPressed(VK_UP)) || (Application::IsKeyPressed(VK_DOWN)))
	{
		Mtx44 rotation2;
		rotation2.SetToRotation(-targetAngle, right.x, right.y, right.z);
		view = rotation2 * view;
	}

	if ((Application::IsKeyPressed('Q')) || (Application::IsKeyPressed('E')))
	{
		Mtx44 rotation3;
		rotation3.SetToRotation(-targetAngle, view.x, view.y, view.z);
		view = rotation3 * view;
	}

	position = (-view * 30) + target;

	target.Set(
		targetx + (up.x * camUpOffset), 
		targety + (up.y * camUpOffset), 
		targetz + (up.z * camUpOffset)
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