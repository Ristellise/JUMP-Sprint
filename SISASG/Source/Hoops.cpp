#include "Hoops.h"

Hoops::Hoops()
{

}

bool Hoops::hoopsCheckXY(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad, int rotationCircle)
{
    // Compare radius of circle with distance of its center from given point
	checkX = x - circle_x;
	checkY = y - circle_y;
	checkZ = z - circle_z;

    if (checkX * checkX + checkY * checkY + checkZ * checkZ <= rad * rad)// && (rotationCircle == 0 || rotationCircle == 45 || rotationCircle == 90))
    {
        return true;
    }
    else
    {
        return false;
    }
}