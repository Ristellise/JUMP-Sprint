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

void spaceship::Update(double dt, float topspeed, float fwdaccl, float bwdaccl, float &accl)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

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

	position = position + view * (float)(velocity * dt);
	view = (target - position).Normalized();
	target = target + view;

	// keeps pitch and yaw within -360 to 360
	if (pitchX > 360)
		pitchX -= 360;
	else if (pitchX < -360)
		pitchX += 360;
	if (yawY > 360)
		yawY -= 360;
	else if (yawY < -360)
		yawY += 360;

	if ((Application::IsKeyPressed('W')) && (velocity < topspeed))
	{
		velocity += (float)(fwdaccl * dt);
		accl = fwdaccl;
	}

	if ((Application::IsKeyPressed('S')) && (velocity > -topspeed))
	{
		velocity += (float)(bwdaccl * dt);
		accl = bwdaccl;
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
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchX += pitch;
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
	velocity = 0;
	yawY = 0;
	pitchX = 0;
}