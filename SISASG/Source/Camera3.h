#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "entity.h"
#include "WindowManager.h"
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
    WindowManager* winMan;

    Camera3();
    ~Camera3();
    virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    virtual void setWindow(WindowManager* winMan);
    virtual void Update(double dt, entity targetShip);
    virtual void Updatealt(double dt);
    virtual void Reset();
};

#endif