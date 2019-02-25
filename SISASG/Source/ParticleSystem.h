#pragma once
#include "genericEntity.h"
#include "Particle.h"
class ParticleSystem : public genericEntity
{
	int numparticles;
	double spawndelay = 0.1;
	Vector3 position;
	Particle particle;
	
public:
    ParticleSystem();
	ParticleSystem(int numparticles);
    ~ParticleSystem();
	void GenerateParticles(double dt);
	void ParticleUpdate(double dt);
	void setplocation(Vector3 position);
	std::vector<Particle> particles;
};

