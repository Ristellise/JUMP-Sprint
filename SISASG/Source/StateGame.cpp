#include "StateGame.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"


StateGame::StateGame()
{
	this->StateName = "Game";
}


StateGame::~StateGame()
{
}

void StateGame::OnEnter()
{
	// Matrix method
	cubeMatrix.SetToIdentity();
	cubeMultR.SetToIdentity();
	cubeMult1.SetToIdentity();
	cubeMult2.SetToIdentity();
	cubeMult3.SetToIdentity();

	Mesh* meshbuffer;

	// Test Cube
	meshbuffer = MeshBuilder::GenerateOBJ("testcube", "OBJ//TestCube.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestCube.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Test Cube
	testCube* testCube1 = new testCube();
	testCube1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	testCube1->type = entityType::eT_Space;
	testCube1->name = "testcube";
	testCube1->physics = true;
	testCube1->Boxsize = BBoxDimensions(2.5f, 2.5f, 2.5f);
	testCube1->meshptr = this->meshGetFast("testcube");
	this->entitylists->push_back(testCube1);

	// Test Env
	meshbuffer = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestEnv.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Test Env
	entity* testEnv = new entity();
	testEnv->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	testEnv->type = entityType::eT_Environment;
	testEnv->name = "testenv";
	// testEnv->physics = true;
	// testEnv->Boxsize = BBoxDimensions(0.f, 0.f, 0.f);
	testEnv->meshptr = this->meshGetFast("testenv");
	this->entitylists->push_back(testEnv);

	// Hoops
	meshbuffer = MeshBuilder::GenerateTorus("hoop", Color(1, 1, 1), 36, 36, 10, 1);
	this->meshList->push_back(meshbuffer);

	// Axes
	meshbuffer = MeshBuilder::GenerateAxes("axes", 200, 200, 200);
	this->meshList->push_back(meshbuffer);
}

void StateGame::OnExit()
{
}

void StateGame::OnUpdate(double dt)
{
	entity* testCube1 = this->entityGetFast("testcube");

	/* start of planet and hoop stuff */

	//test range coords (center sphere coords) (automate later)
	int cx = 0, cy = 0, cz = 0;

	//set position of circle and radius size (automate later)
	int circle_x = 0, circle_y = 0, circle_z = 5, rad = 9;

	if (planetrange1.planetExecuteUI(cx, cy, cz, testCube1->position.x, testCube1->position.y, testCube1->position.z) == true) // checks whether planet and character is in range
	{
		this->dtimestring = "\nYou are in range for a teleport!";
	}
	if (hoop.hoopsExecuteUI(circle_x, circle_y, circle_z, (int)testCube1->position.x, (int)testCube1->position.y, (int)testCube1->position.z, rad) == true)
	{
		this->dtimestring = "\nPassed through circle";
	}

	/* end of planet and hoop stuff*/

	testCube1->Update(dt);

	this->state_cam->Update(dt, *testCube1);

	this->collideInstance->updatingEnts = 0;

	this->collideInstance->doCollisions(*this->entitylists, dt);

	if (Application::IsKeyPressed('R'))
	{
		// Matrix method
		cubeMatrix.SetToIdentity();
		cubeMultR.SetToIdentity();
		cubeMult1.SetToIdentity();
		cubeMult2.SetToIdentity();
		cubeMult3.SetToIdentity();
	}

	if ((Application::IsKeyPressed('5')) && *gameToggle == true && *bounceTime <= 0.0)
	{
		// Matrix method
		cubeMatrix.SetToIdentity();
		cubeMultR.SetToIdentity();
		cubeMult1.SetToIdentity();
		cubeMult2.SetToIdentity();
		cubeMult3.SetToIdentity();

		*gameToggle = false;
		*bounceTime = 0.3;
		this->spawnState = "title";
		this->readyExitlocal = true;
	}

	if ((Application::IsKeyPressed('6')) && *debugToggle == false && *bounceTime <= 0.0)
	{
		*debugToggle = true;
		*bounceTime = 0.3;
		this->spawnState = "debugger";
	}
}

void StateGame::OnRender()
{
	(*this->modelStack).PushMatrix(); // rennder the hoops
	(*this->modelStack).Translate(0, 0, 5); //change coords accordingly (automate later)
	RenderMesh(this->meshGetFast("hoop"), true);
	(*this->modelStack).PopMatrix();

	this->RenderTextScreen(this->meshGetFast("saofontsheet"), this->dtimestring, Color(255, 255, 255), 2.f, 1.f, 24.f);

	for (size_t i = 0; i < this->entitylists->size(); i++)
	{
		(*this->modelStack).PushMatrix();
		entity *buff = (*this->entitylists)[i];
		if (buff->type == entityType::eT_Text)
		{
			buff->position;
			(*this->modelStack).Translate(buff->position.x,
				buff->position.y,
				buff->position.z);
			// rotation coords
			this->RenderText(buff->meshptr, *buff->text, Color(0, 0, 0));
		}
		else if (buff->type == entityType::eT_Object)
		{

			(*this->modelStack).Translate(buff->position.x, buff->position.y, buff->position.z);
			(*this->modelStack).Rotate(buff->yawTotal, 0, 1, 0);
			(*this->modelStack).Rotate(buff->pitchTotal, 1, 0, 0);
			(*this->modelStack).Scale(buff->size.x, buff->size.y, buff->size.z);
			RenderMesh(buff->meshptr, true);

		}
		else if (buff->type == entityType::eT_TextUI)
		{
			this->RenderTextScreen(buff->meshptr, *buff->text, Color(0, 0, 0),
				buff->position.z,	// Used for Text SCaling. only applies to 2d UI 
				buff->position.x,	// Same as before
				buff->position.y);	// Same as before
		}
		else if (buff->type == entityType::eT_Space)
		{
			entity *testCube1 = this->entityGetFast("testcube");

			// Matrix method v2
			(*this->modelStack).PushMatrix();
			cubeMult1.SetToTranslation(testCube1->position.x, testCube1->position.y, testCube1->position.z);

			if (Application::IsKeyPressed(VK_LEFT))
			{
				cubeMultR.SetToRotation(testCube1->angle, testCube1->up.x, testCube1->up.y, testCube1->up.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed(VK_RIGHT))
			{
				cubeMultR.SetToRotation(-(testCube1->angle), testCube1->up.x, testCube1->up.y, testCube1->up.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed(VK_UP))
			{
				cubeMultR.SetToRotation(-(testCube1->angle), testCube1->right.x, testCube1->right.y, testCube1->right.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed(VK_DOWN))
			{
				cubeMultR.SetToRotation(testCube1->angle, testCube1->right.x, testCube1->right.y, testCube1->right.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed('Q'))
			{
				cubeMultR.SetToRotation(-(testCube1->angle), testCube1->view.x, testCube1->view.y, testCube1->view.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed('E'))
			{
				cubeMultR.SetToRotation(testCube1->angle, testCube1->view.x, testCube1->view.y, testCube1->view.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			cubeMult3.SetToScale(5.0f, 5.0f, 5.0f);
			cubeMatrix = cubeMult1 * cubeMult2 * cubeMult3;

			(*this->modelStack).LoadMatrix(cubeMatrix);
			RenderMesh(testCube1->meshptr, true);
			(*this->modelStack).PopMatrix();

            Vector3 Ent2V[] = { buff->HBox.frontLeftUp,
                            buff->HBox.frontLeftDown,
                            buff->HBox.frontRightUp,
                            buff->HBox.frontRightDown,
                            buff->HBox.backLeftUp,
                            buff->HBox.backLeftDown,
                            buff->HBox.backRightUp,
                            buff->HBox.backRightDown };

            for (size_t i = 0; i < 8; i++)
            {
                (*this->modelStack).PushMatrix();
                (*this->modelStack).Translate(Ent2V[i].x, Ent2V[i].y, Ent2V[i].z);

                RenderMesh(this->meshGetFast("debugballs"), true);
                (*this->modelStack).PopMatrix();
            }
		}
		else if (buff->type == entityType::eT_Environment)
		{
			(*this->modelStack).PushMatrix();
			(*this->modelStack).Scale(10.0f, 10.0f, 10.0f);
			RenderMesh(buff->meshptr, true);
			(*this->modelStack).PopMatrix();

			(*this->modelStack).PushMatrix();
			(*this->modelStack).Translate(0, 0, 0);
			RenderMesh(this->meshGetFast("axes"), false);
			(*this->modelStack).PopMatrix();
		}
		(*this->modelStack).PopMatrix();

		
		
	}
}

void StateGame::OnCam(int X, int Y, float XChange, float YChange)
{
}