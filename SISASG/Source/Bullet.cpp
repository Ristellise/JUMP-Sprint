#include "Bullet.h"
#include "Mtx44.h"
#include "Application.h"
#include "spaceship.h"
#include <iostream>

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
	spaceship spaceship1;
	Bullet bullet;

	this->position += view * (float)dt * this->bbSpeed;
	this->timeAlive += (float)dt; //Bullet Update is running, Bullet not updating.

	if (this->timeAlive > 0.5)
	{
		this->position.x = spaceship1.position.x;
		this->position.y = spaceship1.position.y;
		this->position.z = spaceship1.position.z;

		this->target = spaceship1.target * (float)dt;

		std::cout << spaceship1.position.x << std::endl; // 1
		std::cout << spaceship1.position.y << std::endl; // 0
		std::cout << spaceship1.position.z << std::endl; // 0
		std::cout << spaceship1.target << std::endl; // [0, 0, 0]

		this->timeAlive = 0;
	}
} //Spawn after a set amount of time, can only spawn with your holding down a key
// How to code it  to return to the position of the spaceship
// VK_SPACE