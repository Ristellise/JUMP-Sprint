#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "WindowManager.h"
#include "entity.h"
class spaceship : public entity
{
private:

public:
    //Vector3 position;
    //Vector3 target;
    //Vector3 up;

    Vector3 defaultPosition;
    Vector3 defaultTarget;
    Vector3 defaultUp;
    WindowManager* winMan;
    spaceship();
    ~spaceship();
    void setWindow(WindowManager* winMan) { this->winMan = winMan; };
    virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void OnHit(entity* Ent);
    virtual void Update(double dt);

    virtual void Reset();
};

#endif