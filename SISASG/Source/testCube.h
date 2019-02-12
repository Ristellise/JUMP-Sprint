#ifndef TESTCUBE_H
#define TESTCUBE_H

#include "spaceship.h"

class testCube : public spaceship
{	
public:
	float topSpeed;
	float fwdaccl;
	float bwdaccl;
	float accl;

	testCube();
	~testCube();
};

#endif