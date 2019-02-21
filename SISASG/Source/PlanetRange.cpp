#include "PlanetRange.h"


PlanetRange::PlanetRange()
{

}

bool PlanetRange::planetRangeCheck(float cx, float cy, float cz, float x, float y, float z)
{
	float x1 = (float)pow((x - cx), 2);
	float y1 = (float)pow((y - cy), 2);
	float z1 = (float)pow((z - cz), 2);

	// distance between the centre
	// and given point 
	return (x1 + y1 + z1);
}

bool PlanetRange::planetExecuteUI(float cx, float cy, float cz, float x, float y, float z)
{
	//use check function
	float ans = planetRangeCheck(cx, cy, cz, x, y, z);

	//radius sphere
	float r = 150.f;

	if (ans < (r * r) || ans == (r * r))
	{
		return true;
	}
	else
	{
		return false;
	}
}
