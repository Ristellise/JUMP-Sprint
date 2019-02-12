#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

class Light
{
public:
	Light();
	~Light();
	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	//Part 1
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;
private:

};

#endif
