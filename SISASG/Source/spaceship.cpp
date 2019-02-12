#include "spaceship.h"
#include "Application.h"
#include "Mtx44.h"

spaceship::spaceship()
{
	velocity = 0.f;
	pitchX = 0.f;
	yawY = 0.f;
}

spaceship::~spaceship()
{
}

void spaceship::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void spaceship::Update(double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	// if the velocity of the spaceship is >= 0
	// have a natural background deceleration
	if (velocity >= 0.f)
	{
		velocity -= (float)(1.5f * dt);
	}

	/*
	if (Application::IsKeyPressed('A'))
	{
		position = position - right * (float)(velocity * dt);
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		position = position + right * (float)(velocity * dt);
		target = position + view;
	}
	*/

	if (Application::IsKeyPressed('W'))
	{
		velocity += (float)(10.f * dt);
		Vector3 view = (target - position).Normalized();
		position = position + view * (float)(velocity * dt);
		target = target + view;
	}

	if (Application::IsKeyPressed('S'))
	{
		velocity -= (float)(10 * dt);
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(velocity * dt);
		target = target + view;
	}

	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(80.f * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->yawY += yaw;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-80.f * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->yawY += yaw;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(80.f * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchX += pitch;
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-80.f * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchX += pitch;
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

	if (
		(position.x < -200) ||
		(position.x > 200) ||
		(position.y < -200) ||
		(position.y > 200) ||
		(position.z < -200) ||
		(position.z > 200)
		)
	{
		Reset();
	}

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void spaceship::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}