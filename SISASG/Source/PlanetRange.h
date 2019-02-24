#pragma once

#include <iostream>
#include "entity.h"
#include "spaceship.h"

class PlanetRange
{
public:
    PlanetRange();

    bool planetRangeCheck(float cx, float cy, float cz, float x, float y, float z); //these two can be used for teleportation to other planets (rename if necessary)
    bool planetExecuteUI(float cx, float cy, float cz, float x, float y, float z); //these two can be used for teleportation to other planets (rename if necessary)

private:
    

};

