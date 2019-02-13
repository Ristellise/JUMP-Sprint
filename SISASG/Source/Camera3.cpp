#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

// for debugging purposes only
#include <iostream>

Camera3::Camera3()
{
	velocity = 0.f;
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(
	double dt, 
	float targetx, 
	float targety, 
	float targetz,
	float topSpeed, 
	float fwdaccl, 
	float bwdaccl,
	float &accl,
	Vector3 shipView
)
{
	Vector3 shipPos = (targetx, targety, targetz);

	view = (target - position).Normalized();
	right = view.Cross(up);

	this->target.Set(targetx, targety, targetz);

	//
	position = position + shipView * (float)(velocity * dt);
	// view = (target - position).Normalized();
	// target = target + view;

	accl = 0;

	if ((Application::IsKeyPressed('W')) && (velocity < topSpeed))
	{
		velocity += (float)(fwdaccl * dt);
		accl = fwdaccl;
	}

	if ((Application::IsKeyPressed('S')) && (velocity > -topSpeed))
	{
		velocity += (float)(bwdaccl * dt);
		accl = bwdaccl;
	}
	//

	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(80.f * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-80.f * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(-80.f * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(80.f * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}

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
	velocity = 0;
}