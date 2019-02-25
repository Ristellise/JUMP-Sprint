#pragma once
#include "genericEntity.h"
class Particle : public genericEntity
{
	float particleLifespan;

public:
	Particle();
	~Particle();
	void pSetlocation(Vector3 Position, float pitch, float yaw, float roll);
	bool pUpdate(double dt);
	Vector3 Position;
	float Pitch;
	float Yaw;
	float Roll;
};

