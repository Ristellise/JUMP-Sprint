#include "Hoops.h"

Hoops::Hoops()
{

}

bool Hoops::hoopsCheckXY(float circle_x, float circle_y, float circle_z, float x, float y, float z, float rad) // almost works just need to add z axis somehow so renamed to XY for now
{
	// Compare radius of circle with distance of its center from given point
	if ((x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad && z == circle_z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Hoops::hoopsExecuteUI(float circle_x, float circle_y, float circle_z, float x, float y, float z, float rad)
{
	bool ans = hoopsCheckXY(circle_x, circle_y, circle_z, x, y, z,rad);

	if (ans == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}