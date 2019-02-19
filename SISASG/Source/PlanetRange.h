#pragma once

#include <iostream>
#include "entity.h"
#include "testCube.h"

class PlanetRange
{
public:
	PlanetRange();

	int planetRangeCheck(int cx, int cy, int cz, int x, int y, int z); //these two can be used for teleportation to other planets (rename if necessary)
	int planetExecuteUI(int cx, int cy, int cz, int x, int y, int z); //these two can be used for teleportation to other planets (rename if necessary)

private:
	

};

