/*#include "Bullet.h"

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::InitialiseBullets()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		BulletArray[i] = 0;
	}
}

Bullet * Bullet::CreateBullet(float x, float y, float xDir, float yDir, float speed)
{
	Bullet *bullet = new Bullet();
	bullet->xPos = x;
	bullet->yPos = y;
	bullet->xDir = xDir;
	bullet->yDir = yDir;
	bullet->speed = speed;
	return bullet;
}

void Bullet::DestroyBullet(Bullet * pBullet)
{
	delete pBullet;
}

bool Bullet::Shoot(float x, float y, float xDir, float yDir, float speed)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (BulletArray[i] == 0)
		{
			BulletArray[i] = CreateBullet(x, y, xDir, yDir, speed);
			return true;
		}
	}
}

void Bullet::UpdateBullets()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (BulletArray[i] == 0)
		{
			continue;
		}

		BulletArray[i]->xPos += BulletArray[i]->xDir * BulletArray[i]->speed;
		BulletArray[i]->yPos += BulletArray[i]->yDir * BulletArray[i]->speed;
	}
}*/

// Create a OpenGL model of bullet, set it to a fixed coordinate, try to animate it using coords.**