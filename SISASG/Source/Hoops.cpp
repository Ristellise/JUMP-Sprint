#include "Hoops.h"

Hoops::Hoops()
{

}

int Hoops::hoopsCheckXY(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad) // almost works just need to add z axis somehow so renamed to XY for now
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

int Hoops::hoopsExecuteUI(int circle_x, int circle_y, int circle_z, int x, int y, int z, int rad)
{
	testCube *testCube1 = new testCube();

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