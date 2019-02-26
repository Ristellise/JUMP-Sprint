#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	Rotate = true;
	x = 1;
	y = 1;
	z = 1;
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
		spawndelay = spawndelaydefault;
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
		  (refPoint.right * offset_x)
		+ (refPoint.up * offset_y)
		+ (refPoint.view * offset_z)
		+  refPoint.position;
}

void ParticleSystem::setRotatestatus(bool rtate)
{
	this->Rotate = rtate;
}

void ParticleSystem::setScale(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}



