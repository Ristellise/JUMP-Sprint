#include "Bullet.h"
#include "Mtx44.h"
#include "Application.h"
#include "testCube.h"

Bullet::Bullet()
{
}

Bullet::Bullet(const Vector3 & pos, const Vector3 & target, const Vector3 & up)
{
	timeAlive = 0;
	this->position = pos;
	this->target = target;
	this->up = up;
	this->view = (target - position).Normalized();
}

Bullet::~Bullet()
{
}

void Bullet::Init(const Vector3 & pos, const Vector3 & target, const Vector3 & up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	this->view = (target - position).Normalized();

}

void Bullet::Update(double dt)
{
	if (BulletActive = true)
	{
		testCube testCube1;

		this->position += view * dt * this->bbSpeed;
		this->timeAlive += dt;

		if (this->timeAlive > 0.5)
		{
			this->position = testCube1.position;
			this->target = testCube1.target;
			//timeAlive = 0;
		}
	}
} //Spawn after a set amount of time, can only spawn with your holding down a key
// How to code it  to return to the position of the spaceship
// VK_SPACE