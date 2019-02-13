#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Mesh.h"

class entity
{
public:
	float velocity;
	float topSpeed;
	float fwdaccl;
	float bwdaccl;
	float accl;

	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 view;
	Vector3 right;

	Mesh *meshptr = NULL;

	entity();
	~entity();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif