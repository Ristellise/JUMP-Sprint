#pragma once
#ifndef Uniform



#include "Mesh.h"
enum UNIFORM_TYPE
{
    U_MVP = 0,
    U_MODELVIEW,
    U_MODELVIEW_INVERSE_TRANSPOSE,
    U_MATERIAL_AMBIENT,
    U_MATERIAL_DIFFUSE,
    U_MATERIAL_SPECULAR,
    U_MATERIAL_SHININESS,

    U_NUMLIGHTS,
    U_LIGHT0_POSITION,
    U_LIGHT0_TYPE,
    U_LIGHT0_COLOR,
    U_LIGHT0_POWER,
    U_LIGHT0_KC,
    U_LIGHT0_KL,
    U_LIGHT0_KQ,
    U_LIGHT0_COSCUTOFF,
    U_LIGHT0_COSINNER,
    U_LIGHT0_EXPONENT,
    U_LIGHT0_SPOTDIRECTION,

    U_LIGHTENABLED,
    U_COLOR_TEXTURE_ENABLED,
    U_COLOR_TEXTURE,
    U_TEXT_ENABLED,
    U_TEXT_COLOR,
    U_TOTAL,
};

enum GEOMETRY_TYPE
{
    GEO_AXES,
    GEO_LIGHTBALL,
    GEO_TEXT, // To move to vector solution

    // Skybox
    GEO_LEFT,
    GEO_RIGHT,
    GEO_FRONT,
    GEO_BACK,
    GEO_TOP,
    GEO_BOTTOM,

    // Test Cube
    GEO_TESTCUBE,

	// Test Environment
	GEO_TESTENV,

    //Planets
    GEO_PLANET_VENUS,
    GEO_PLANET_EARTH,
    GEO_PLANET_MARS,
    GEO_PLANET_JUPITER,
    GEO_SUN,

	// Bullet
	GEO_BULLETBODY,

	//Asteroids
	GEO_ASTEROID1,

	//test car
	GEO_CAR,

    NUM_GEOMETRY, // This should be at the end of the Enum, else nothing renders.
};

#endif // !Uniform