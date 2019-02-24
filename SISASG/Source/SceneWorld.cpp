#include "SceneWorld.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "StateInit.h"
#include "State.h"
#include "StateStat.h"
#include "StateGame.h"
#include "StateMenus.h"
#include "StateHangar.h"
#include "StateShop.h"
#include "StatePlanet.h"
#include <stdexcept>



SceneWorld::SceneWorld()
{
    throw std::runtime_error("Did not define window correctly!");
}

SceneWorld::SceneWorld(WindowManager* winMan)
{
    this->l_window = winMan;
}

SceneWorld::~SceneWorld()
{
}

void SceneWorld::Init()
{
    // Vector method
    currRotate = 3;
    prevRotate = 3;

    srand(unsigned int(time(NULL)));

    random = rand() % 10 + 1;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    rotateAngle = 0;
    movement_asteroid1_z = 0;
    // Generate a default VAO for now
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    // bullet.Init(Vector3(spaceship.position.x, spaceship.position.y, spaceship.position.z),Vector3(0,0,-1),Vector3(0,1,0));
    // camera.Init(Vector3(0, 4, -30), Vector3(0, 4, 1), Vector3(0, 1, 0));
    // spaceship.Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));

    this->Mouse = MouseHandler(20.0f);
    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
    projectionStack.Init();
    projectionStack.LoadMatrix(projection);

    //Load vertex and fragment shaders
    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

    // Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

    // Use our shader
    glUseProgram(m_programID);

    lights[0].type = Light::LIGHT_DIRECTIONAL;
    lights[0].position.Set(0.f, 0.f, -50.0f);

    glUniform1i(m_parameters[U_NUMLIGHTS], 8);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

    // meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 200, 200, 200);
    camera.setWindow(this->l_window);
    FLInstance.Loadfnt("Font/fnt.fnt");
    Stateinit* initInstance = new Stateinit();
    StateStat* statstate = new StateStat();
    StateGame* gamestate = new StateGame();
    StateMenus* menustate = new StateMenus();
    StateHangar* hangarstate = new StateHangar();
    StateShop* shopstate = new StateShop();
    StatePlanet* planetstate = new StatePlanet();
    this->StateManInst.SetMatrixes(&this->modelStack, &this->viewStack, &this->projectionStack);
    this->StateManInst.addAvailable(initInstance);
    this->StateManInst.addAvailable(statstate);
    this->StateManInst.addAvailable(gamestate);
    this->StateManInst.addAvailable(menustate);
    this->StateManInst.addAvailable(hangarstate);
    this->StateManInst.addAvailable(shopstate);
    this->StateManInst.addAvailable(planetstate);
    this->StateManInst.setCam(&camera);
    this->StateManInst.Init(this->m_parameters, &this->FLInstance, &this->Mouse);

    //** FontLoader Instance **//

    //// The fontsheet on a big mesh
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("saofontsheet", this->FLInstance);
    meshList[GEO_TEXT]->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);

#pragma region Skybox

    meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

#pragma endregion

#pragma region Planets

    meshList[GEO_PLANET_VENUS] = MeshBuilder::GenerateOBJ("venus", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_VENUS]->textureID = LoadTGA("TGA//venus texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_PLANET_EARTH] = MeshBuilder::GenerateOBJ("earth", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_EARTH]->textureID = LoadTGA("TGA//earth texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_PLANET_MARS] = MeshBuilder::GenerateOBJ("mars", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_MARS]->textureID = LoadTGA("TGA//mars texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_PLANET_JUPITER] = MeshBuilder::GenerateOBJ("jupiter", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_JUPITER]->textureID = LoadTGA("TGA//jupiter texture.tga", GL_LINEAR, GL_CLAMP);

#pragma endregion

    //asteroids
    //meshList[GEO_ASTEROID1] = MeshBuilder::GenerateOBJ("asteroid1", "OBJ//asteroid1.obj")[0];
    //meshList[GEO_ASTEROID1]->textureID = LoadTGA("TGA//asteroid1 texture.tga", GL_LINEAR, GL_CLAMP);

    // Test Cube
    //meshList[GEO_spaceship] = MeshBuilder::GenerateOBJ("spaceship", "OBJ//spaceship.obj")[0];
    //meshList[GEO_spaceship]->textureID = LoadTGA("TGA//spaceship.tga", GL_LINEAR, GL_CLAMP);
    //meshList[GEO_spaceship] = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship2.obj")[0];
    //meshList[GEO_spaceship]->textureID = LoadTGA("TGA//Ship2.tga", GL_LINEAR, GL_CLAMP);

    // Lightball
    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightBall", Color(1, 1, 1), 9, 36, 1);

    //Bullet
    meshList[GEO_BULLETBODY] = MeshBuilder::GenerateSphere("bulletbody", Color(255, 255, 255), 18, 36, 0.5);
    
    // Test Environment
    // meshList[GEO_TESTENV] = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];

    // Lightball
    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightBall", Color(1, 1, 1), 9, 36, 1);
}

void SceneWorld::Update(double dt)
{
    this->Mouse.Update(this->l_window->handle,dt);

    static const float LSPEED = 10.0f;

    // For culling and line / fill modes
    if (this->l_window->IsKeyPressed('1'))
    {
        glEnable(GL_CULL_FACE);
    }
    if (this->l_window->IsKeyPressed('2'))
    {
        glDisable(GL_CULL_FACE);
    }
    if (this->l_window->IsKeyPressed('3'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (this->l_window->IsKeyPressed('4'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (this->l_window->IsKeyPressed(GLFW_KEY_SPACE))
    {
    }
    this->lastkeypress += dt;

    static int rotateDir = 1;
    static int rotateDir_asteroid = 1;
    static const float ROTATE_SPEED = 10.f;
    rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

    movement_asteroid1_z += (float)(rotateDir_asteroid * ROTATE_SPEED * dt);

    if (movement_asteroid1_z * rotateDir_asteroid > 40)
    {
        rotateDir_asteroid = -rotateDir_asteroid;
    }
    if (movement_asteroid1_z < -40 && rotateDir_asteroid < -40)
    {
        rotateDir_asteroid = -rotateDir_asteroid;
    }

    this->StateManInst.Update(dt, this->l_window->handle);

    glfwGetWindowSize(l_window->handle, &WindowXpos, &WindowYpos); // gets the size of the window
    Mtx44 projection;
    projection.SetToPerspective(45.f, (float)WindowXpos / WindowYpos, 0.1f, ViewRange); // Window Scaling
    projectionStack.LoadMatrix(projection);
}

void SceneWorld::RenderMesh(Mesh *mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    if (enableLight)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView = viewStack.Top() * modelStack.Top();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }
    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }
    mesh->Render();
    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

// Main Render loop
void SceneWorld::Render()
{
    //Clear color & depth buffer every frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewStack.LoadIdentity();

    viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
    modelStack.LoadIdentity();

    if (lights[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
            &lightDirection_cameraspace.x);
    }
    else if (lights[0].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
            &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() *
            lights[0].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
            &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
            &lightPosition_cameraspace.x);
    }

    this->StateManInst.Render();
}

/*-------------
| - Self Renderer Works... but derpy.
-------------*/
void SceneWorld::RenderTextScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
        return;
    //glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); // No need for camera ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); // Reset modelStack
    modelStack.Scale(size, size, size);
    modelStack.Translate(x, y, 0);

    float advance = 0;
    float yadvance = 0.0f;
    // 
    charData buffer;
    FontResult res;
    for (unsigned i = 0; i < text.length(); ++i)
    {
        if (text[i] != '\n')
        {
            Mtx44 characterSpacing;
            res = this->FLInstance.getFontData((unsigned int)text[i]);
            buffer = res.font;
            characterSpacing.SetToTranslation(advance, yadvance, 0);
            Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
            glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
            advance += 1.0f / (float)buffer.advance + 0.3f; // advance the text
            mesh->Render(res.index * 6, 6); // count is the index Size
        }
        else
        {
            yadvance -= 1.0f;
            advance = 0.0f;
        }

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    //glEnable(GL_DEPTH_TEST);
}

void SceneWorld::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
        return;
    //glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);


    float advance = 0;
    float yadvance = 0.0f;
    // 
    charData buffer;
    FontResult res;
    for (unsigned i = 0; i < text.length(); ++i)
    {
        if (text[i] != '\n')
        {
            Mtx44 characterSpacing;
            res = this->FLInstance.getFontData((unsigned int)text[i]);
            buffer = res.font;
            characterSpacing.SetToTranslation(advance, yadvance, 0);
            Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
            glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
            advance += 1.0f / (float)buffer.advance + 0.3f; // advance the text
            mesh->Render(res.index * 6, 6); // count is the index Size
        }
        else
        {
            yadvance -= 1.0f;
            advance = 0.0f;
        }

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    //glEnable(GL_DEPTH_TEST);
}

void SceneWorld::Exit()
{
    // Cleanup VBO here
    this->StateManInst.Shutdown();
    glDeleteProgram(m_programID);

}