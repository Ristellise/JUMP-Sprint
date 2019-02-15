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
}

void Camera3::Update(
	double dt, 
	float targetx, 
	float targety, 
	float targetz
)
{
	view = target - position;
	right = view.Cross(up).Normalized();
	up = right.Cross(view).Normalized();
	this->target.Set(targetx, targety, targetz);
	position = target - view;

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