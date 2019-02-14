#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Mesh.h"

enum entityType
{
    eT_Object,
    eT_Text,
    eT_TextUI,
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
    entityType type;

	Vector3 right;	// relative x
	Vector3 up;		// relative y
	Vector3 view;	// relative z

    std::string *text;
    std::string name = "";
	Mesh *meshptr = nullptr;

	entity();
	~entity();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif