/*#pragma once
#ifndef BULLET_H
#define BULLET_H

class Bullet
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
	void UpdateBullets();
	const int MAX_BULLETS = 30;
};

#endif*/