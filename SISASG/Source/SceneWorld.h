#ifndef SCENE_SKBOX_H
#define SCENE_SKBOX_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "MouseHandler.h"
#include "Uniforms.h"
class SceneWorld : public Scene
{

    enum GEOMETRY_TYPE
    {
        GEO_AXES,
        GEO_LIGHTBALL,
        GEO_TEXT, // Temporary.
        GEO_LID, // 1 Animation
        GEO_TEDDY, // Character
        GEO_DOOR,
        GEO_LEFT,
        GEO_RIGHT,
        GEO_FRONT,
        GEO_BACK,
        GEO_TOP,
        GEO_BOTTOM,
        NUM_GEOMETRY, // This should be at the end of the Enum, else nothing renders.
    };

public:
    SceneWorld();
    SceneWorld(GLFWwindow * l_window);
    ~SceneWorld();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    void RenderText(Mesh* mesh, std::string text, Color color);
    virtual void Exit();
    void SetWindow(GLFWwindow* window);
private:
    GLFWwindow* l_window;
    unsigned m_vertexArrayID;
    Mesh* meshList[NUM_GEOMETRY];
    std::vector<Mesh*> meshListVector;
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    float rotateStar;

    Camera3 camera;
    MS modelStack, viewStack, projectionStack;
    Light lights[8];
    FontLoader FLInstance;
    MouseHandler Mouse;
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

    unsigned int selector = 2;
};

#endif