#pragma once

#include "entity.h"
#include "spaceship.h"

class Hoops
{
public:
    Hoops();

    bool hoopsCheckXY(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad, int rotationCircle); // for hoops detection (not fully working)
    

private:
	int checkX = 0, checkY = 0, checkZ = 0;
};