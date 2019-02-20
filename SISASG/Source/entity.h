#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"
#include "Mesh.h"

enum entityType
{
    eT_Object,
    eT_Text,
    eT_TextUI,
    eT_Ship,
	eT_Environment,
    eT_Count
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

class BBoxDimensions
{
public:
    BBoxDimensions();
    ~BBoxDimensions();
    unsigned int BBForward;
    unsigned int BBSide;
    unsigned int BBTop;
    BBoxDimensions(unsigned int forward, unsigned int side, unsigned int top) { Set(forward, side, top); };
    void Set(unsigned int forward, unsigned int side, unsigned int top);
};

class entity
{
public:
	float topSpeed;
	float fwdaccl;
	float bwdaccl;

	float velocity;
	float accl;

	float angle;
	float yawTotal;
	float pitchTotal;
	float rollTotal;

    bool physics = false;

	Vector3 position;
	Vector3 target;
    entityType type;
    BBoxDimensions Boxsize;
	hitbox HBox;

    Vector3 size;

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

    void UpdateBBox();

    /*
	virtual void Update(
        double dt,
        float topSpeed,
        float fwdaccl,
        float bwdaccl,
        float &accl
    );
	*/
};

#endif