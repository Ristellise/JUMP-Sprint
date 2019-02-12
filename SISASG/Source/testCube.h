#ifndef TESTCUBE_H
#define TESTCUBE_H

#include "spaceship.h"

class testCube : public spaceship
{
private:
	float topSpeed;
	float acceleration;

public:
	testCube();
	~testCube();
};

#endif