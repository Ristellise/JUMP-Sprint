#pragma once

#include "entity.h"
#include "spaceship.h"

class Hoops
{
public:
	Hoops();

	bool hoopsCheckXY(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad); // for hoops detection (not fully working)
	bool hoopsExecuteUI(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad); // for hoops detection (not fully working)

private:

};