#pragma once
#include "genericEntity.h"
class Particle
{
	Vector3 Position;
	Vector3 Target;
	Vector3 Up;
	float particleLifespan;
public:
	Particle(Vector3 Position,Vector3 Target,Vector3 Up);
	~Particle();
};

