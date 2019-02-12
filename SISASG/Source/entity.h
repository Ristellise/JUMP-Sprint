#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"

class entity
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	entity();
	~entity();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif