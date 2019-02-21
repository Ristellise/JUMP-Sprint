#include "StateGame.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"
#include "genericEntity.h"


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
	testCube1->type = entityType::eT_Ship;
	testCube1->name = "testcube";
	testCube1->physics = true;
	testCube1->Boxsize = BBoxDimensions(2.5f, 2.5f, 2.5f);
	testCube1->meshptr = this->meshGetFast("testcube");
	this->entitylists->push_back(testCube1);

	// Test Env
	meshbuffer = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestEnv.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

    this->audiosrc.Load("Audio/testtrack.flac");
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


    // Collision tester

    entity* current = new genericEntity();

    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(0, 0, 1), Vector3(0, 1, 0));
    current->type = entityType::eT_Object;
    current->meshptr = this->meshGetFast("testcube");
    current->physics = true;
    current->Boxsize = BBoxDimensions(0.5f, 0.5f, 0.5f);
    this->entitylists->push_back(current);
    //this->STData->VERYLOUD.play();
}

void StateGame::OnExit()
{
}

void StateGame::OnUpdate(double dt)
{
	entity* testCube1 = this->entityGetFast("testcube");

	///////* start of planet and hoop stuff *///////

	//test range coords for planet range (center sphere coords) (automate later)

	int cx_venus = 300, cy_venus = 0, cz_venus = -300; // can manually set planet coords due to generate ones already
	int cx_earth = -400, cy_earth = 0, cz_earth = -400;
	int cx_mars = -550, cy_mars = 0, cz_mars = 550;
	int cx_jupiter = 800, cy_jupiter = 0, cz_jupiter = 800;

	// checks whether planet and character is in range (venus)

	this->dtimestring = "Points : ";
	this->dtimestring += std::to_string(points);
	this->dtimestring += "\nFPS:";
	this->dtimestring += std::to_string(1.0f / dt);
	this->dtimestring += "\nCam X:";
	this->dtimestring += std::to_string(this->state_cam->position.x);
	this->dtimestring += "\nCam Y:";
	this->dtimestring += std::to_string(this->state_cam->position.y);
	this->dtimestring += "\nCam Z:";
	this->dtimestring += std::to_string(this->state_cam->position.z);

	if (planetrange1.planetExecuteUI(cx_venus, cy_venus, cz_venus, (int)testCube1->position.x, (int)testCube1->position.y, (int)testCube1->position.z) == true)
	{
		if (Application::IsKeyPressed(VK_RETURN)) // testing keypress inside range checker
		{
			testCube1->position.x = 0, testCube1->position.y = 0, testCube1->position.z = 0;
		}
		this->dtimestring += "\nYour range check should work for Venus";
	}
	if (planetrange1.planetExecuteUI(cx_earth, cy_earth, cz_earth, (int)testCube1->position.x, (int)testCube1->position.y, (int)testCube1->position.z) == true)
	{
		this->dtimestring += "\nYour range check should work for Earth";
	}
	if (planetrange1.planetExecuteUI(cx_mars, cy_mars, cz_mars, (int)testCube1->position.x, (int)testCube1->position.y, (int)testCube1->position.z) == true)
	{
		this->dtimestring += "\nYour range check should work for Mars";
	}
	if (planetrange1.planetExecuteUI(cx_jupiter, cy_jupiter, cz_jupiter, (int)testCube1->position.x, (int)testCube1->position.y, (int)testCube1->position.z) == true)
	{
		this->dtimestring += "\nYour range check should work for Jupiter";
	}

	// generates the hoop checkers
	for (int i = 0; i < 20; i++) // for loop follows array
	{
		// passes values into hoops for coords

		if (hoop.hoopsExecuteUI(offset_x[i], offset_y[i], offset_z[i], (int)testCube1->position.x, (int)testCube1->position.y, (int)testCube1->position.z, rad) == true)
		{
			points++;
		}
	}
	
	
	///////* end of planet and hoop stuff *///////

    for (size_t i = 0; i < this->entitylists->size(); i++)
    {
        (*this->entitylists)[i]->Update(dt);
    }

	//testCube1->Update(dt);

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

	if ((Application::IsKeyPressed('5')) && this->STData->gameToggle == true && this->STData->bounceTime <= 0.0)
	{
		// Matrix method
		cubeMatrix.SetToIdentity();
		cubeMultR.SetToIdentity();
		cubeMult1.SetToIdentity();
		cubeMult2.SetToIdentity();
		cubeMult3.SetToIdentity();

        this->STData->gameToggle = false;
        this->STData->bounceTime = 0.3;
		this->spawnState = "title";
		this->readyExitlocal = true;
	}

	if ((Application::IsKeyPressed('6')) && this->STData->debugToggle == false && this->STData->bounceTime <= 0.0)
	{
        this->STData->debugToggle = true;
        this->STData->bounceTime = 0.3;
		this->spawnState = "debugger";
	}
}

void StateGame::OnRender()
{
	///////* start of hoops *///////

	// for loop to create 5 hoops?

	int the_addition = 10;

	// venus 300, 0, -300, 20.0f, 20.0f, 20.0f

	if (x = 250, y = 0, z = -250) // this double checks if specified start coordinates are right
	{
		for (int i = 0; i < 5; i++) // for loop follows amount of rings wanted inside the "map" (e.g 0 to 4 for this case, thus 5 hoops)
		{
			offset_x[i] = x + the_addition * 2; // changes x coord (can multiply / divide all these to make it more spaced out)
			offset_y[i] = y + the_addition / 2; // changes y coord
			offset_z[i] = z + the_addition; // changes z coord

			(*this->modelStack).PushMatrix(); // render the hoops
			(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); // sets the coords of each hoop (coord stored in an array for each hoop)
			RenderMesh(this->meshGetFast("hoop"), true);
			(*this->modelStack).PopMatrix();
			the_addition += 20; // increases addition value so it keeps going

		}
	}
	
	// earth -400, 0, -400, 21.0f, 21.0f, 21.0f

	x = -350, y = 0, z = -350, the_addition = 15; // sets coords for next hoop range (near earth)

	if (x = -350, y = 0, z = -350)
	{
		for (int i = 5; i < 10; i++)
		{
			offset_x[i] = x + the_addition;
			offset_y[i] = y + the_addition;
			offset_z[i] = z + the_addition;

			(*this->modelStack).PushMatrix(); // render the hoops
			(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); 
			RenderMesh(this->meshGetFast("hoop"), true);
			(*this->modelStack).PopMatrix();
			the_addition += 25;
		}
	}

	// mars -550, 0, 550, 15.0f, 15.0f, 15.0f

	x = -600, y = 10, z = 500, the_addition = 5; // sets coords for next hoop range (near mars)

	if (x = -600, y = 10, z = 500)
	{
		for (int i = 10; i < 15; i++)
		{
			offset_x[i] = x + the_addition;
			offset_y[i] = y + the_addition;
			offset_z[i] = z + the_addition;

			(*this->modelStack).PushMatrix(); // render the hoops
			(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]);
			RenderMesh(this->meshGetFast("hoop"), true);
			(*this->modelStack).PopMatrix();
			the_addition += 40;
		}
	}

	// jupiter 800, 0, 800, 75.0f, 75.0f, 75.0f

	x = 900, y = 50, z = 850, the_addition = 20; // sets coords for next hoop range (near jupiter)

	if (x = 900, y = 50, z = 850)
	{
		for (int i = 15; i < 20; i++)
		{
			offset_x[i] = x + the_addition;
			offset_y[i] = y + the_addition;
			offset_z[i] = z + the_addition;

			(*this->modelStack).PushMatrix(); // render the hoops
			(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); //change coords accordingly (automate later)
			RenderMesh(this->meshGetFast("hoop"), true);
			(*this->modelStack).PopMatrix();
			the_addition += 15;
		}
	}

	///////* end of hoops *///////

	this->RenderTextScreen(this->meshGetFast("saofontsheet"), this->dtimestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 2.f, 1.f, 24.f);

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
		else if (buff->type == entityType::eT_Ship)
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
}

void StateGame::OnCam(int X, int Y, float XChange, float YChange)
{
}