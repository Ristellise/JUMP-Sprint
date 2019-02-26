#pragma once
#include "genericEntity.h"
class asteroidEnt :
    public genericEntity
{
public:
    asteroidEnt();
    void OnHit(entity* Ent);
    ~asteroidEnt();
};

