#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "entity.h"

class spaceship : public entity
{
private:
	float pitch;
	float yaw;
	float acceleration;

public:
	spaceship();
	~spaceship();

};

#endif