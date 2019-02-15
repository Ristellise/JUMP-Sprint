#include "SceneWorld.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "StateInit.h"
#include "State.h"
#include <stdexcept>



SceneWorld::SceneWorld()
{
    throw std::runtime_error("Did not define window correctly!");
}

SceneWorld::SceneWorld(GLFWwindow *l_window)
{
    this->l_window = l_window;
}

SceneWorld::~SceneWorld()
{
}

void SceneWorld::Init()
{
	cubeRotate = 0;

	srand(unsigned int(time(0)));

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

    
	bullet.Init(Vector3(testCube1.position.x, testCube1.position.y, testCube1.position.z),Vector3(0,0,-1),Vector3(0,1,0));
    camera.Init(Vector3(0, 10, -30), Vector3(0, 0, 0), Vector3(0, 1, 0));
    testCube1.Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));

    this->Mouse = MouseHandler(20.0f);
    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
    projectionStack.LoadMatrix(projection);

    //Load vertex and fragment shaders
    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
    sf.loadFile("test.ini");
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
    lights[0].position.Set(100.0f, 100.0f, 100.0f);
    lights[0].color.Set(1, 1, 1);
    lights[0].power = 1.f;
    lights[0].kC = 1.f;
    lights[0].kL = 0.01f;
    lights[0].kQ = 0.001f;
    lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
    lights[0].cosInner = cos(Math::DegreeToRadian(30));
    lights[0].exponent = 3.f;
    lights[0].spotDirection.Set(0.f, 1.f, 0.f);

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

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 200, 200, 200);

    FLInstance.Loadfnt("Font/fnt.fnt");
    Stateinit* initInstance = new Stateinit();
    this->StateManInst.SetMatrixes(&this->modelStack, &this->viewStack, &this->projectionStack);
    this->StateManInst.addAvailable(initInstance);
    this->StateManInst.setCam(&camera);
    this->StateManInst.Init(this->m_parameters, &this->FLInstance, &this->Mouse);

    //** FontLoader Instance **//


    //// The fontsheet on a big mesh
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("saofontsheet", this->FLInstance);
    meshList[GEO_TEXT]->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);

    //skybox
    meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom skybox", Color(128 / 255.f, 128 / 255.f, 128 / 255.f), 1.f);

    //planets
    meshList[GEO_PLANET_VENUS] = MeshBuilder::GenerateOBJ("venus", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_VENUS]->textureID = LoadTGA("TGA//venus texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_PLANET_EARTH] = MeshBuilder::GenerateOBJ("earth", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_EARTH]->textureID = LoadTGA("TGA//earth texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_PLANET_MARS] = MeshBuilder::GenerateOBJ("mars", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_MARS]->textureID = LoadTGA("TGA//mars texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_PLANET_JUPITER] = MeshBuilder::GenerateOBJ("jupiter", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_PLANET_JUPITER]->textureID = LoadTGA("TGA//jupiter texture.tga", GL_LINEAR, GL_CLAMP);

    meshList[GEO_SUN] = MeshBuilder::GenerateOBJ("sun", "OBJ//Planet sphere.obj")[0];
    meshList[GEO_SUN]->textureID = LoadTGA("TGA//sun texture.tga", GL_LINEAR, GL_CLAMP);

    //asteroids
    meshList[GEO_ASTEROID1] = MeshBuilder::GenerateOBJ("asteroid1", "OBJ//asteroid1.obj")[0];
    meshList[GEO_ASTEROID1]->textureID = LoadTGA("TGA//asteroid1 texture.tga", GL_LINEAR, GL_CLAMP);

    // Test Cube
    meshList[GEO_TESTCUBE] = MeshBuilder::GenerateOBJ("testcube", "OBJ//TestCube.obj")[0];
    meshList[GEO_TESTCUBE]->textureID = LoadTGA("TGA//TestCube.tga", GL_LINEAR, GL_CLAMP);

	// Lightball
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightBall", Color(1, 1, 1), 9, 36, 1);

	//Bullet
	meshList[GEO_BULLETBODY] = MeshBuilder::GenerateSphere("bulletbody", Color(255, 255, 255), 18, 36, 0.5);
	// Test Environment
	meshList[GEO_TESTENV] = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];

    //test car
    meshList[GEO_CAR] = MeshBuilder::GenerateOBJ("testcar", "OBJ//Car.obj")[0];
    meshList[GEO_CAR]->textureID = LoadTGA("TGA//car.tga", GL_LINEAR, GL_CLAMP);

    // Lightball
    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightBall", Color(1, 1, 1), 9, 36, 1);

}

void SceneWorld::Update(double dt)
{
    // Temporarily commented out from Josh mouse handler
    this->Mouse.Update(this->l_window,dt);
    // this->Mouse.Center(this->l_window);

    static const float LSPEED = 10.0f;

    // For culling and line / fill modes
    if (Application::IsKeyPressed('1'))
    {
        glEnable(GL_CULL_FACE);
    }
    if (Application::IsKeyPressed('2'))
    {
        glDisable(GL_CULL_FACE);
    }
    if (Application::IsKeyPressed('3'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (Application::IsKeyPressed('4'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (Application::IsKeyPressed(VK_SPACE))
    {
    }
    if (Application::IsKeyPressed('I'))
        lights[this->selector].position.z -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('K'))
        lights[this->selector].position.z += (float)(LSPEED * dt);
    if (Application::IsKeyPressed('J'))
        lights[this->selector].position.x -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('L'))
        lights[this->selector].position.x += (float)(LSPEED * dt);
    if (Application::IsKeyPressed('O'))
        lights[this->selector].position.y -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('P'))
        lights[this->selector].position.y += (float)(LSPEED * dt);

	if ((Application::IsKeyPressed(VK_LEFT)) || (Application::IsKeyPressed(VK_RIGHT)))
	{
		cubeRotate = 0;
	}
	if ((Application::IsKeyPressed(VK_UP)) || (Application::IsKeyPressed(VK_DOWN)))
	{
		cubeRotate = 1;
	}
	if ((Application::IsKeyPressed('Q')) || (Application::IsKeyPressed('E')))
	{
		cubeRotate = 2;
	}

    this->lastkeypress += dt;

    camera.Update(
        dt, 
        testCube1.position.x, 
        testCube1.position.y, 
        testCube1.position.z,
		testCube1.topSpeed, 
		testCube1.fwdaccl, 
		testCube1.bwdaccl,
		testCube1.view
    );

    testCube1.Update(dt);

    this->dtimestring = "FPS:";
    this->dtimestring += std::to_string(1.0f / dt);
    this->dtimestring += "\nCam X:";
    this->dtimestring += std::to_string(this->camera.position.x);
    this->dtimestring += "\nCam Y:";
    this->dtimestring += std::to_string(this->camera.position.y);
    this->dtimestring += "\nCam Z:";
    this->dtimestring += std::to_string(this->camera.position.z);
    this->dtimestring += "\n"	+ std::to_string(this->lights[this->selector].position.x) + " |" 
                                + std::to_string(this->lights[this->selector].position.y) + " |"
                                + std::to_string(this->lights[this->selector].position.z);
    this->dtimestring += "\nVel :";
    this->dtimestring += std::to_string(testCube1.velocity);
    this->dtimestring += "\nAcl :";
    this->dtimestring += std::to_string(testCube1.accl);
    this->dtimestring += "\nPit :";
    this->dtimestring += std::to_string(testCube1.pitchTotal);
    this->dtimestring += "\nYaw :";
    this->dtimestring += std::to_string(testCube1.yawTotal);
	this->dtimestring += "\nCamVel :";
	this->dtimestring += std::to_string(camera.velocity);
	this->dtimestring += "\nCamAcl :";
	this->dtimestring += std::to_string(camera.accl);

	this->dtimestring += "\nCubeUpX:";
	this->dtimestring += std::to_string(testCube1.up.x);
	this->dtimestring += "\nCubeUpY:";
	this->dtimestring += std::to_string(testCube1.up.y);
	this->dtimestring += "\nCubeUpZ:";
	this->dtimestring += std::to_string(testCube1.up.z);

	this->dtimestring += "\nCubeRightX:";
	this->dtimestring += std::to_string(testCube1.right.x);
	this->dtimestring += "\nCubeRightY:";
	this->dtimestring += std::to_string(testCube1.right.y);
	this->dtimestring += "\nCubeRightZ:";
	this->dtimestring += std::to_string(testCube1.right.z);

	this->dtimestring += "\nCubeViewX:";
	this->dtimestring += std::to_string(testCube1.view.x);
	this->dtimestring += "\nCubeViewY:";
	this->dtimestring += std::to_string(testCube1.view.y);
	this->dtimestring += "\nCubeViewZ:";
	this->dtimestring += std::to_string(testCube1.view.z);

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

    this->StateManInst.Update(dt, this->l_window);
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

static const int SKYBOXSIZE = 2000;

void SceneWorld::RenderSkybox()
{
    float x = this->camera.position.x;
    float y = this->camera.position.y;
    float z = this->camera.position.z;

    modelStack.PushMatrix();
    modelStack.Translate(x - SKYBOXSIZE / 2, y, z);
    modelStack.Rotate(90, 0, 1, 0);
    modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    RenderMesh(meshList[GEO_LEFT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(x + SKYBOXSIZE / 2, y, z);
    modelStack.Rotate(90, 0, 1, 0);
    modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    RenderMesh(meshList[GEO_RIGHT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(x, y, z + SKYBOXSIZE / 2);
    modelStack.Rotate(90, 0, 0, 1);
    modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    RenderMesh(meshList[GEO_FRONT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(x, y, z - SKYBOXSIZE / 2);
    modelStack.Rotate(90, 0, 0, 1);
    modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    RenderMesh(meshList[GEO_BACK], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(x, y + SKYBOXSIZE / 2, z);
    modelStack.Rotate(90, 1, 0, 0);
    modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    RenderMesh(meshList[GEO_TOP], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(x, y - SKYBOXSIZE / 2, z);
    modelStack.Rotate(90, 1, 0, 0);
    modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
    RenderMesh(meshList[GEO_BOTTOM], false);
    modelStack.PopMatrix();

}

void SceneWorld::RenderBullet()
{
	modelStack.PushMatrix();

	modelStack.Translate(testCube1.position.x, testCube1.position.y, testCube1.position.z);
	modelStack.Rotate(testCube1.yawTotal, testCube1.up.x, testCube1.up.y, testCube1.up.z);
	modelStack.Rotate(testCube1.pitchTotal, testCube1.right.x, testCube1.right.y, testCube1.right.z);

	RenderMesh(meshList[GEO_BULLETBODY], true);
	modelStack.PopMatrix();
}

void SceneWorld::RenderPlanets()
{
    //venus
    modelStack.PushMatrix();
    modelStack.Translate(300, 0, -300);
    modelStack.Rotate(rotateAngle, 0, 1, 0);
    modelStack.Scale(20.0f, 20.0f, 20.0f);
    RenderMesh(meshList[GEO_PLANET_VENUS], true);
    modelStack.PopMatrix();

    //earth
    modelStack.PushMatrix();
    modelStack.Translate(-400, 0, -400);
    modelStack.Rotate(rotateAngle, 0, 1, 0);
    modelStack.Scale(21.0f, 21.0f, 21.0f);
    RenderMesh(meshList[GEO_PLANET_EARTH], true);
    modelStack.PopMatrix();

    //mars
    modelStack.PushMatrix();
    modelStack.Translate(-550, 0, 550);
    modelStack.Rotate(rotateAngle, 0, 1, 0);
    modelStack.Scale(15.0f, 15.0f, 15.0f);
    RenderMesh(meshList[GEO_PLANET_MARS], true);
    modelStack.PopMatrix();

    //jupiter
    modelStack.PushMatrix();
    modelStack.Translate(800, 0, 800);
    modelStack.Rotate(rotateAngle, 0, 1, 0);
    modelStack.Scale(75.0f, 75.0f, 75.0f);
    RenderMesh(meshList[GEO_PLANET_JUPITER], true);
    modelStack.PopMatrix();

    //sun
    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 0);
    modelStack.Rotate(rotateAngle, 0, 1, 0);
    modelStack.Scale(170.0f, 170.0f, 170.0f);
    RenderMesh(meshList[GEO_SUN], true);
    modelStack.PopMatrix();
}

void SceneWorld::RenderAsteroid()
{
		modelStack.PushMatrix();
		modelStack.Translate((float)random, (float)random, (float)random);
		modelStack.Translate(movement_asteroid1_z, 0, 0);
		modelStack.Rotate(rotateAngle * 5, 1, 0, 1);
		modelStack.Scale(0.8f, 0.8f, 0.8f);
		RenderMesh(meshList[GEO_ASTEROID1], true);
		modelStack.PopMatrix();	
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

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 0);
    RenderMesh(meshList[GEO_AXES], false);
    modelStack.PopMatrix();

     RenderSkybox();

     RenderPlanets();

	 RenderAsteroid();

    // testcar
    // modelStack.PushMatrix();
    // modelStack.Translate(-10, 0, 0);
    // RenderMesh(meshList[GEO_CAR], true);
    // modelStack.PopMatrix();

    modelStack.PushMatrix();
	modelStack.Translate(testCube1.position.x, testCube1.position.y, testCube1.position.z);
	switch (cubeRotate)
	{
	case 0:
		modelStack.Rotate(testCube1.yawTotal, testCube1.up.x, testCube1.up.y, testCube1.up.z);
		break;
	case 1:
		modelStack.Rotate(testCube1.pitchTotal, testCube1.right.x, testCube1.right.y, testCube1.right.z);
		break;
	case 2:
		modelStack.Rotate(testCube1.rollTotal, testCube1.view.x, testCube1.view.y, testCube1.view.z);
		break;
	}
    modelStack.Scale(5.0f, 5.0f, 5.0f);
    RenderMesh(meshList[GEO_TESTCUBE], true);
    modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Scale(10.0f, 10.0f, 10.0f);
	RenderMesh(meshList[GEO_TESTENV], true);
	modelStack.PopMatrix();*/

    modelStack.PushMatrix();
    modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
    RenderMesh(meshList[GEO_LIGHTBALL], false);
    modelStack.PopMatrix();

    RenderTextScreen(meshList[GEO_TEXT], this->dtimestring, Color(255, 255, 0), 2, 1.f, 24.f);
    this->StateManInst.Render();

	if (Application::IsKeyPressed('F'))
	{
		RenderBullet();
		//Make it shoot?

	}
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