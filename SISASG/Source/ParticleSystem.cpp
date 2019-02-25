#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::GenerateParticles(double dt, float pitch, float yaw, float roll)
{
	spawndelay -= dt;
	if (0 >= spawndelay)
	{
		particle.pSetlocation(position, pitch, yaw, roll);
		particles.push_back(particle);
		spawndelay = 0.001;
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

void ParticleSystem::setplocation(entity refPoint,float offset_x,float offset_y,float offset_z)
{
	this->position = 
		(refPoint.right * offset_x) + 
		(refPoint.up * offset_y) + 
		(refPoint.view * offset_z) + 
		refPoint.position;
}
