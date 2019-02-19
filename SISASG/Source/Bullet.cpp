#include "Bullet.h"
#include "Mtx44.h"
#include "Application.h"
#include "testCube.h"

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
	this->view = (target - position).Normalized(); //This makes the program unable to run
}

void Bullet::Update(double dt)
{
	Bullet* bullet = new Bullet();
	this->position += view * dt * this->bbSpeed;
	this->timeAlive += dt;
	if (this->timeAlive > 5)
	{
		// TODO: Call destructor.
		this->doDestroy = true;
	}
}