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
	this->position += view * dt * this->bbSpeed;
	this->timeAlive += dt; //Bullet Update is running, Bullet not updating.
}