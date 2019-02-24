#include "Particle.h"
// temp
// extends from entity, onupdate and such


Particle::Particle(Vector3 Position, Vector3 Target, Vector3 Up)
{
	this->Position = Position;
	particleLifespan = 10;
}

Particle::~Particle()
{
}
