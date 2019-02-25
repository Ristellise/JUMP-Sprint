#include "ParticleSystem.h"
// temp


ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(int numparticles)
{
	this->numparticles = numparticles;
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::GenerateParticles(double dt)
{
	spawndelay -= dt;
	if (0 >= spawndelay)
	{
		particle.pSetlocation(position);
		particles.push_back(particle);
		spawndelay = 0.005;
	}
}

void ParticleSystem::ParticleUpdate(double dt)
{
	
	int i = particles.size();
	while (i > 0)
	{
		i--;
		if (particles[i].pUpdate(dt))
		{
			particles.erase(particles.begin() + i);
		}

	}
}

void ParticleSystem::setplocation(Vector3 position)
{
	this->position = position;
}
