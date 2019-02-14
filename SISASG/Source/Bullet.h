#pragma once
#ifndef BULLET_H
#define BULLET_H
#include "entity.h"

class Bullet : public entity
{
	Bullet *BulletArray[100];
	float xPos, yPos, xDir, yDir, speed;
	float timeAlive;
public:
	Bullet();
	~Bullet();
	void InitialiseBullets();
	Bullet *CreateBullet(float x, float y, float xDir, float yDir, float speed);
	void DestroyBullet(Bullet *pBullet);
	bool Shoot(float x, float y, float xDir, float yDir, float speed);
	void UpdateBullets(float dt);
	void DrawBullets();
	const int MAX_BULLETS = 30;
	double  g_dElapsedTime = 0.0;
	double  g_dDeltaTime = 0.0;
};

#endif