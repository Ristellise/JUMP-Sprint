#pragma once
#ifndef BULLET_H
#define BULLET_H
#include "genericEntity.h"

class Bullet : public genericEntity
{
	double bbSpeed = 1200.0;
public:
    Bullet();
    Bullet(const Vector3 & pos, const Vector3 & target, const Vector3 & up);
    ~Bullet();
    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Update(double dt);
    float timeAlive = 0;
    //bool BulletActive = false;
};

#endif