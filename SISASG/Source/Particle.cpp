#include "Particle.h"
// temp
// extends from entity, onupdate and such


Particle::Particle()
{
	
}

Particle::~Particle()
{
}

void Particle::pSetlocation(Vector3 Position)
{
	this->Position = Position;
	//this->Position.x += ((rand() % 5) - 10.f);
	//this->Position.y += ((rand() % 5) - 10.f);
	//this->Position.z += ((rand() % 5) - 10.f);
	particleLifespan = 0.5f;
}

bool Particle::pUpdate(double dt)
{
	particleLifespan -= dt;
	if (0 > particleLifespan)
		return true;
	else
		return false;
}
