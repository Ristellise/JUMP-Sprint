#pragma once
#include "genericEntity.h"
#include "Particle.h"
class ParticleSystem : public genericEntity
{
	double spawndelaydefault = 0.001;
	double spawndelay = spawndelaydefault;
	Particle particle;
public:
    ParticleSystem();
    ~ParticleSystem();
	void GenerateParticles(double dt);
	void ParticleUpdate(double dt);
	void setplocation(entity position, float offset_x, float offset_y, float offset_z);
	void setRotatestatus(bool rtate);
	void setScale(float x, float y, float z);
	Vector3 position;
	std::vector<Particle> particles;
	bool Rotate;
	float x;
	float y;
	float z;
};

