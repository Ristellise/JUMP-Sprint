#include "Particle.h"

Particle::Particle()
{

}

Particle::~Particle()
{
}

void Particle::pSetlocation(Vector3 Position)
{
	this->Position = Position;
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
