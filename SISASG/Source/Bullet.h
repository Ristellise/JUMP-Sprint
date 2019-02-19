#pragma once
#ifndef BULLET_H
#define BULLET_H
#include "entity.h"

class Bullet : public entity
{
	float timeAlive;
	double bbSpeed = 30.0;
	int BulletLeft[10];
public:
	Bullet();
	~Bullet();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Update(double dt);
	bool isFiring = false;
};

#endif