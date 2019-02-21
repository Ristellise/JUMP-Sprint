#include "spaceship.h"
#include "Application.h"
#include "Mtx44.h"
#include <limits>

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
	angle = 0.f;
	pitchTotal = 0.f;
	yawTotal = 0.f;
	rollTotal = 0.f;
}

void spaceship::Update(double dt)
{
	angle = (float)(80.f * dt);
	position = position + view * (float)(velocity * dt);
	target += view * 10.0f; // we premultiply the view so that we dont fuck it up.
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
    if (velocity > 0 && 
        !(Application::IsKeyPressed('W')) && (
        (Application::IsKeyPressed(VK_LEFT)) ||
        (Application::IsKeyPressed(VK_RIGHT)) ||
        (Application::IsKeyPressed(VK_DOWN)) ||
        (Application::IsKeyPressed(VK_UP)) || 
        (Application::IsKeyPressed('Q')) || 
        (Application::IsKeyPressed('E'))
            )
        )
    {
        velocity -= (float)(drift * dt);
    }
    
	if (Application::IsKeyPressed(VK_LEFT))
	{
		view = (target - position).Normalized();
		right = view.Cross(up);
		Mtx44 rotation;
		rotation.SetToRotation(angle, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->yawTotal += angle;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(-angle, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->yawTotal -= angle;
	}

	if (Application::IsKeyPressed(VK_DOWN))
	{
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(angle, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchTotal += angle;
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(-angle, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->pitchTotal -= angle;
	}

	if (Application::IsKeyPressed('Q'))
	{
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(-angle, view.x, view.y, view.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->rollTotal -= angle;
	}

	if (Application::IsKeyPressed('E'))
	{
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(angle, view.x, view.y, view.z);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		this->rollTotal += angle;
	}
    
    if (this->yawTotal + angle >= 360.0f)
    {
        this->yawTotal = 360.0f - (this->yawTotal + angle);
    }
    else if (this->yawTotal + angle <= -360.0f)
    {
        this->yawTotal = 360.0f + (this->yawTotal + angle);
    }

    if (this->pitchTotal + angle >= 360.0f)
    {
        this->pitchTotal = 360.0f - (this->pitchTotal + angle);
    }
    else if (this->pitchTotal + angle <= -360.0f)
    {
        this->pitchTotal = 360.0f + (this->pitchTotal + angle);
    }

	if (this->rollTotal + angle >= 360.0f)
	{
		this->rollTotal = 360.0f - (this->rollTotal + angle);
	}
	else if (this->rollTotal + angle <= -360.0f)
	{
		this->rollTotal = 360.0f + (this->rollTotal + angle);
	}

	if (
		(position.x < -std::numeric_limits<float>::max()) ||
		(position.x > std::numeric_limits<float>::max()) ||
		(position.y < -std::numeric_limits<float>::max()) ||
		(position.y > std::numeric_limits<float>::max()) ||
		(position.z < -std::numeric_limits<float>::max()) ||
		(position.z > std::numeric_limits<float>::max())
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