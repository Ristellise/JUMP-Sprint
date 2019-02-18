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

struct BBoxDimensions
{
    unsigned int BBForward;
    unsigned int BBSide;
    unsigned int BBTop;
};
struct hitbox
{
    Vector3 frontLeftUp;
    Vector3 frontLeftDown;
    Vector3 frontRightDown;
    Vector3 frontRightUp;
    Vector3 backLeftUp;
    Vector3 backLeftDown;
    Vector3 backRightDown;
    Vector3 backRightUp;
};


class entity
{
public:
	float velocity;
	float topSpeed;
	float fwdaccl;
	float bwdaccl;
	float accl;

	float angle;
	float yawTotal;
	float pitchTotal;
	float rollTotal;

    // Physics
    bool enablePhysics = false;
    BBoxDimensions Boxsize;
    hitbox HBox;

    // View shit
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
	void Update(double dt);
    void UpdateBBox();
};

#endif