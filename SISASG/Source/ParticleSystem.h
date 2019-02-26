#pragma once
#include "genericEntity.h"
#include "Particle.h"
class ParticleSystem : public genericEntity
{
	double spawndelay = 0.005;
	Particle particle;
public:
    ParticleSystem();
    ~ParticleSystem();
	void GenerateParticles(double dt);
	void ParticleUpdate(double dt);
	void setplocation(entity position, float offset_x, float offset_y, float offset_z);
	Vector3 position;

	std::vector<Particle> particles;
};

