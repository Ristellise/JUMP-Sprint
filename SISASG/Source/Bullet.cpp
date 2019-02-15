#include "Bullet.h"
#include "Mtx44.h"
#include "Application.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::Init(const Vector3 & pos, const Vector3 & target, const Vector3 & up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	//this->view = (target - position).Normalized();
}

void Bullet::Update(double dt)
{
	//if (Application::IsKeyPressed('F'))
	{
		this->position += view * dt * this->bbSpeed;
			this->timeAlive += dt;
			if (this->timeAlive > 5)
			{
				// TODO: Call destructor.
				this->doDestroy = true;
			}
	}
}