#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	pRotateAngle = 1.f;
	scale_x = 1;
	scale_y = 1;
	scale_z = 1;
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
		spawndelay = spawndelaydefault; // see particlesystem.h for the spawndelaydefault
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

void ParticleSystem::setRotateangle(float rtate)
{
	this->pRotateAngle = rtate;
}

void ParticleSystem::setScale(float scale_x, float scale_y, float scale_z)
{
	this->scale_x = scale_x;
	this->scale_y = scale_y;
	this->scale_z = scale_z;
}



