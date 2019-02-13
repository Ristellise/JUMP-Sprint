#ifndef SCENE_SKBOX_H
#define SCENE_SKBOX_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "testCube.h"

#include "MouseHandler.h"
#include "Uniforms.h"
#include "StateManager.h"

class SceneWorld : public Scene
{

public:
    SceneWorld();
    SceneWorld(GLFWwindow * l_window);
    ~SceneWorld();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    void RenderSkybox();
    void RenderPlanets();
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
    virtual void Exit();
    void SetWindow(GLFWwindow* window);
private:
    GLFWwindow* l_window;
    unsigned m_vertexArrayID;
    
    std::vector<Mesh*> meshListVector;
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    Camera3 camera;
    testCube testCube1;
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
    float rotateAngle;
    bool leg = false;
    float legdance = 0.0f;
    StateManager StateManInst;
    float redlightpow = 0.0f;
    float bluelightpow = 0.0f;
    Mesh* meshList[NUM_GEOMETRY];

    unsigned int selector = 0;

	//RenderBullet
	void RenderBullet();
};

#endif