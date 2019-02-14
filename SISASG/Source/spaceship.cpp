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
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	velocity = 0.f;
	yaw = 0.f;
	pitch = 0.f;
	pitchTotal = 0.f;
	yawTotal = 0.f;
}

void spaceship::Update(double dt)
{
	position = position + view * (float)(velocity * dt);
	target += view;
	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();
	up = right.Cross(view).Normalized();
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
    
	if (Application::IsKeyPressed(VK_LEFT))
	{
        yaw = (float)(80.f * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->yawTotal += yaw;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		yaw = (float)(-80.f * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(yaw, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->yawTotal += yaw;
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		pitch = (float)(80.f * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchTotal += pitch;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		pitch = (float)(-80.f * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchTotal += pitch;
	}

	if (Application::IsKeyPressed('Q'))
	{
		roll = (float)(-80.f * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(roll, view.x, view.y, view.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->rollTotal += roll;
	}

	if (Application::IsKeyPressed('E'))
	{
		roll = (float)(80.f * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(roll, view.x, view.y, view.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->rollTotal += roll;
	}

    if (this->yawTotal + yaw >= 360.0f)
    {
        this->yawTotal = 360.0f - (this->yawTotal + yaw);
    }
    else if (this->yawTotal + yaw <= -360.0f)
    {
        this->yawTotal = 360.0f + (this->yawTotal + yaw);
    }

    if (this->pitchTotal + pitch >= 360.0f)
    {
        this->pitchTotal = 360.0f - (this->pitchTotal + pitch);
    }
    else if (this->pitchTotal + pitch <= -360.0f)
    {
        this->pitchTotal = 360.0f + (this->pitchTotal + pitch);
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
	yawTotal = 0;
	pitchTotal = 0;
	rollTotal = 0;
}