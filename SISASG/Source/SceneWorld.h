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
    double lastkeypress = 0.0;
    float rotateAngle;
    StateManager StateManInst;
    Mesh* meshList[NUM_GEOMETRY];

    unsigned int selector = 0;
};

#endif