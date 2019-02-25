#include "Particle.h"
// temp
// extends from entity, onupdate and such


Particle::Particle()
{
	
}

Particle::~Particle()
{
}

void Particle::pSetlocation(Vector3 Position, float pitch, float yaw, float roll)
{
	this->Position = Position;
	Pitch = pitch;
	Yaw = yaw;
	Roll = roll;
	particleLifespan = 1.5f;
}

bool Particle::pUpdate(double dt)
{
	particleLifespan -= (float)dt;
	if (0 > particleLifespan)
		return true;
	else
		return false;
}
