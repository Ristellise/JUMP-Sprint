#pragma once

#include "entity.h"
#include "spaceship.h"

class Hoops
{
public:
	Hoops();

	bool hoopsCheckXY(float circle_x, float circle_y, float circle_z, float x, float y, float z, float rad); // for hoops detection (not fully working)
	bool hoopsExecuteUI(float circle_x, float circle_y, float circle_z, float x, float y, float z, float rad); // for hoops detection (not fully working)

private:

};