#pragma once


class collision
{

public:
	collision();
	~collision();
	int checkCollision();
	void doCollision();

private:
	bool collisionX;
	bool collisionY;
	bool collisionZ;

};

