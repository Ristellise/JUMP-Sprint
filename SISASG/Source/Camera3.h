#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "entity.h"

class Camera3 : public entity
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	float camUpOffset;

	float camX;
	float camY;
	float camZ;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(
		double dt, 
		float targetx, 
		float targety, 
		float targetz,
		float targetYaw,
		float targetPitch,
		float targetRoll,
		Vector3 targetRight,
		Vector3 targetUp,
		Vector3 targetView
	);
	virtual void Updatealt(double dt);
	virtual void Reset();
};

#endif