#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Mesh.h"

enum entityType
{
    eT_Object,
    eT_Text,
    eT_TextUI,
	eT_Space,
    eT_Count
};

class entity
{
public:
	float velocity;
	float topSpeed;
	float fwdaccl;
	float bwdaccl;
	float accl;
	float yaw;
	float pitch;
	float yawTotal;
	float pitchTotal;

	Vector3 position;
	Vector3 target;
	Vector3 up;
    entityType type;

	Vector3 view;
	Vector3 right;

    std::string *text = new std::string();
    std::string name = "";
	Mesh *meshptr = nullptr;

	entity();
	~entity();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
	virtual void Update(
		double dt,
		float topSpeed,
		float fwdaccl,
		float bwdaccl,
		float &accl
	);
};

#endif