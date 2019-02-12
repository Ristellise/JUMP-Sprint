#ifndef SCENE_SKBOX_H
#define SCENE_SKBOX_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "testCube.h"

class SceneWorld : public Scene
{
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

        NUM_GEOMETRY, // This should be at the end of the Enum, else nothing renders.
    };

public:
    SceneWorld();
    ~SceneWorld();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
    virtual void Exit();

private:
    unsigned m_vertexArrayID;
    Mesh* meshList[NUM_GEOMETRY];
    std::vector<Mesh*> meshListVector;
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    Camera3 camera;
	testCube testCube1;
    MS modelStack, viewStack, projectionStack;
    Light lights[8];
    FontLoader FLInstance;

    std::string dtimestring = "";
    void RenderMesh(Mesh *mesh, bool enableLight);
    float doormovement = 0.0f;
    float lidmovement = 0.0f;
    bool opendoor = false;
    bool openlid = false;
    bool renable = false;
    double lastkeypress = 0.0;

    bool leg = false;
    float legdance = 0.0f;

    float redlightpow = 0.0f;
    float bluelightpow = 0.0f;

    unsigned int selector = 0;
};

#endif