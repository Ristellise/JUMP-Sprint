#pragma once
#include "genericEntity.h"
class Particle : public genericEntity
{
	
	float particleLifespan;
public:
	Particle();
	~Particle();
	void pSetlocation(Vector3 Position);
	bool pUpdate(double dt);
	Vector3 Position;
};

