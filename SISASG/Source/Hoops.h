#pragma once

#include "entity.h"
#include "testCube.h"

class Hoops
{
public:
	Hoops();

	int hoopsCheckXY(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad); // for hoops detection (not fully working)
	int hoopsExecuteUI(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad); // for hoops detection (not fully working)

private:

};