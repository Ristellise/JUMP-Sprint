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
	pitchTotal = 0.f;
	yawTotal = 0.f;
}

void spaceship::Update(
	double dt,  
	float topSpeed,
	float fwdaccl,
	float bwdaccl, 
	float &accl
)
{
	view = (target - position).Normalized();
	right = view.Cross(up);

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
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		right = view.Cross(up);
		this->yawTotal += yaw;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		yaw = (float)(-80.f * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		right = view.Cross(up);
		this->yawTotal += yaw;

        if (this->yawTotal + yaw >= 360.0f)
        {
            this->yawTotal = 360.0f - (this->yawTotal + yaw);
        }
        else if (this->yawTotal + yaw <= -360.0f)
        {
            this->yawTotal = 360.0f + (this->yawTotal + yaw);
        }
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		pitch = (float)(80.f * dt);
		// Vector3 view = (target - position).Normalized();
		// Vector3 right = view.Cross(up);
		// right.y = 0;
		// right.Normalize();
		// up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		right = view.Cross(up);
		this->pitchTotal += pitch;

        if (this->pitchTotal + pitch >= 360.0f)
        {
            this->pitchTotal = 360.0f - (this->pitchTotal + pitch);
        }
        else if (this->pitchTotal + pitch <= -360.0f)
        {
            this->pitchTotal = 360.0f + (this->pitchTotal + pitch);
        }
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		pitch = (float)(-80.f * dt);
		// Vector3 view = (target - position).Normalized();
		// Vector3 right = view.Cross(up);
		// right.y = 0;
		// right.Normalize();
		// up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		right = view.Cross(up);
		this->pitchTotal += pitch;
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
}