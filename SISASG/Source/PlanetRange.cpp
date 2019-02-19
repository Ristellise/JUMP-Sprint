#include "PlanetRange.h"


PlanetRange::PlanetRange()
{

}

int PlanetRange::planetRangeCheck(int cx, int cy, int cz, int x, int y, int z)
{
	int x1 = (int)pow((x - cx), 2);
	int y1 = (int)pow((y - cy), 2);
	int z1 = (int)pow((z - cz), 2);

	// distance between the centre  
	// and given point 
	return (x1 + y1 + z1);
}

int PlanetRange::planetExecuteUI(int cx, int cy, int cz, int x, int y, int z)
{
	testCube *testCube1 = new testCube();

	//use check function
	int ans = planetRangeCheck(cx, cy, cz, x, y, z);

	//radius sphere
	int r = 50;

	if (ans < (r * r) || ans == (r * r))
	{
		return true;
	}
	else
	{
		return false;
	}
}
