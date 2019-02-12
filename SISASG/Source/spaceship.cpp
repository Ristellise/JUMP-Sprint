#include "spaceship.h"
#include "Application.h"
#include "Mtx44.h"

spaceship::spaceship()
{
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

	static const float SPACESHIP_SPEED = 100.f;

	if (Application::IsKeyPressed('A'))
	{
		position = position - right * (float)(40.f * dt);
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		position = position + right * (float)(40.f * dt);
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		position = position + view * (float)(40.f * dt);
		target = target + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(40.f * dt);
		target = target + view;
	}

	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(SPACESHIP_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-SPACESHIP_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(SPACESHIP_SPEED * dt);
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
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-SPACESHIP_SPEED * dt);
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