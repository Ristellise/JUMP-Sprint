#pragma once
#include "entity.h"
class genericEntity :
    public entity
{
public:
    genericEntity();
    ~genericEntity();

    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Init(const Vector3 & pos, const Vector3 & target, const Vector3 & up, const Vector3 & size);
    void Reset();
    void Update(double dt);
};

