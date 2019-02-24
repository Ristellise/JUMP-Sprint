#ifndef SCENE_SKBOX_H
#define SCENE_SKBOX_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "spaceship.h"
#include "Bullet.h"
#include "SaveFiles.h"
#include "entity.h"

#include "MouseHandler.h"
#include "Uniforms.h"
#include "StateManager.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <vector>

class SceneWorld : public Scene
{

public:
    SceneWorld();
    SceneWorld(GLFWwindow * l_window);
    ~SceneWorld();

    int WindowXpos = 0;
    int WindowYpos = 0;
    float ViewRange = 10000.f;

    int prevRotate;
    int currRotate;

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    void RenderSkybox();
    void RenderPlanets();
    void RenderAsteroid();
    // void RenderSpaceship();
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
    virtual void Exit();
    // void SetWindow(GLFWwindow* window);

private:
    GLFWwindow* l_window;
    unsigned m_vertexArrayID;
    
    std::vector<Mesh*> meshListVector;
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    Camera3 camera;
    // spaceship spaceship;
    MS modelStack, viewStack, projectionStack;
    Light lights[8];
    FontLoader FLInstance;
    MouseHandler Mouse;
    INIFile sf;
    Bullet bullet;
    entity entity;
    std::string dtimestring = "";
    void RenderMesh(Mesh *mesh, bool enableLight);
    //bool checkerAsteroid = false;
    int random;
    int asteroidSpawn = 0;
    double lastkeypress = 0.0;
    float rotateAngle;
    float movement_asteroid1_z;
    bool leg = false;
    float legdance = 0.0f;
    StateManager StateManInst;
    Mesh* meshList[NUM_GEOMETRY];

    unsigned int selector = 0;

    //RenderBullet
    // void RenderBullet();

    std::vector <int> cubeRotateVector;
};

#endif