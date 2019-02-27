#pragma once
#ifndef BULLET_H
#define BULLET_H
#include "genericEntity.h"

class Bullet : public genericEntity
{
	
public:
    Bullet();
    Bullet(const Vector3 & pos, const Vector3 & target, const Vector3 & up);
    ~Bullet();
    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Update(double dt);
    void ReleaseRecollections(entity ** bullet2);
    void OnDelete(entity **Ent);
    double timeAlive = 0;
    double bbSpeed = 500.0;
    //bool BulletActive = false;
};

#endif