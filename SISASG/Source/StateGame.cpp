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

	// Ship loading
	switch (this->STData->shipSelect)
	{
	case 0:
		// Ship1 - Starter ship
		meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship1.obj")[0];
		meshbuffer->textureID = LoadTGA("TGA//Ship1.tga", GL_LINEAR, GL_CLAMP);
		break;
	case 1:
		// Ship2 - Heavy ship
		meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship2.obj")[0];
		meshbuffer->textureID = LoadTGA("TGA//Ship2.tga", GL_LINEAR, GL_CLAMP);
		break;
	case 2:
		// Ship3 - Speed ship
		// meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship3.obj")[0];
		// meshbuffer->textureID = LoadTGA("TGA//Ship3.tga", GL_LINEAR, GL_CLAMP);
		break;
	}
	
	this->meshList->push_back(meshbuffer);

	// Spaceship
	spaceship* spaceship1 = new spaceship();
	spaceship1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	spaceship1->type = entityType::eT_Ship;
	spaceship1->name = "spaceship";
	spaceship1->physics = true;
	spaceship1->Boxsize = BBoxDimensions(2.5f, 2.5f, 2.5f);
	spaceship1->meshptr = this->meshGetFast("spaceship");
	spaceship1->topSpeed = 20.0f;
	spaceship1->fwdaccl = 10.f;
	spaceship1->bwdaccl = -5.f;
	spaceship1->drift = 10.0f;
	this->entitylists->insert_or_assign("spaceship1",spaceship1);

	// Test Env
	meshbuffer = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestEnv.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

    this->audiosrc.Load("Audio/testtrack.flac",SourceType::ST_WAVSTREAM);
	// Test Env
	entity* testEnv = new entity();
	testEnv->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	testEnv->type = entityType::eT_Environment;
	testEnv->name = "testenv";
	// testEnv->physics = true;
	// testEnv->Boxsize = BBoxDimensions(0.f, 0.f, 0.f);
	testEnv->meshptr = this->meshGetFast("testenv");
	this->entitylists->insert_or_assign("testenv",testEnv);

	// Hoops
	meshbuffer = MeshBuilder::GenerateTorus("hoop", Color(255 / 255.f, 255 / 255.f, 255 / 255.f), 36, 36, 10, 1);
	this->meshList->push_back(meshbuffer);

	// Axes
	meshbuffer = MeshBuilder::GenerateAxes("axes", 200, 200, 200);
	this->meshList->push_back(meshbuffer);


    // Collision tester

    entity* current = new genericEntity();

    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(0, 0, 1), Vector3(0, 1, 0));
    current->type = entityType::eT_Object;
    current->meshptr = this->meshGetFast("spaceship");
    current->physics = true;
    current->Boxsize = BBoxDimensions(0.5f, 0.5f, 0.5f);
    this->entitylists->insert_or_assign("testcube",current);
    //this->STData->VERYLOUD.play();
}

void StateGame::OnExit()
{
}

void StateGame::OnUpdate(double dt)
{
	entity* spaceship = this->entityGetFast("spaceship1");

	///////* start of planet and hoop stuff *///////

	//test range coords for planet range (center sphere coords) (automate later)

	float cx_venus = 300.f, cy_venus = 0.f, cz_venus = -300.f; // can manually set planet coords due to generate ones already
	float cx_earth = -400.f, cy_earth = 0.f, cz_earth = -400.f;
	float cx_mars = -550.f, cy_mars = 0.f, cz_mars = 550.f;
	float cx_jupiter = 800.f, cy_jupiter = 0.f, cz_jupiter = 800.f;

	// checks whether planet and character is in range (venus)
	/*
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

	if (planetrange1.planetExecuteUI(cx_venus, cy_venus, cz_venus, spaceship->position.x, spaceship->position.y, spaceship->position.z) == true)
	{
		if (Application::IsKeyPressed(VK_RETURN)) // testing keypress inside range checker1`
		{
			spaceship->position.x = 0, spaceship->position.y = 0, spaceship->position.z = 0;
		}
		this->dtimestring += "\nYour range check should work for Venus";
	}
	if (planetrange1.planetExecuteUI(cx_earth, cy_earth, cz_earth, spaceship->position.x, spaceship->position.y, spaceship->position.z) == true)
	{
		this->dtimestring += "\nYour range check should work for Earth";
	}
	if (planetrange1.planetExecuteUI(cx_mars, cy_mars, cz_mars, spaceship->position.x, spaceship->position.y, spaceship->position.z) == true)
	{
		this->dtimestring += "\nYour range check should work for Mars";
	}
	if (planetrange1.planetExecuteUI(cx_jupiter, cy_jupiter, cz_jupiter, spaceship->position.x, spaceship->position.y, spaceship->position.z) == true)
	{
		this->dtimestring += "\nYour range check should work for Jupiter";
	}
	*/

	// generates the hoop checkers
	for (int i = 0; i < 20; i++) // for loop follows array
	{
		// passes values into hoops for coords

		if (hoop.hoopsExecuteUI(offset_x[i], offset_y[i], offset_z[i], spaceship->position.x, spaceship->position.y, spaceship->position.z, rad) == true)
		{
			points++;
		}
	}
	
	
	///////* end of planet and hoop stuff *///////

    std::map<std::string, entity*>::iterator it;
    for (it = this->entitylists->begin(); it != this->entitylists->end(); it++)
    {
        it->second->Update(dt);
    }

	//spaceship->Update(dt);

	this->state_cam->Update(dt, *spaceship);

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

	if (x = 0.f, y = 0.f, z = 30.f)
	{
		for (int i = 0; i < 5; i++)
		{
			offset_x[i] = x;
			offset_y[i] = y;
			offset_z[i] = z + the_addition;

			(*this->modelStack).PushMatrix(); // render the hoops
			(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); // sets the coords of each hoop (coord stored in an array for each hoop)
			RenderMesh(this->meshGetFast("hoop"), true);
			(*this->modelStack).PopMatrix();
			the_addition += 20; // increases addition value so it keeps going
		}
	}

	// venus 300, 0, -300, 20.0f, 20.0f, 20.0f

	//if (x = 250, y = 0, z = -250) // this double checks if specified start coordinates are right
	//{
	//	for (int i = 0; i < 5; i++) // for loop follows amount of rings wanted inside the "map" (e.g 0 to 4 for this case, thus 5 hoops)
	//	{
	//		offset_x[i] = x + the_addition * 2;	// changes x coord (can multiply / divide all these to make it more spaced out)
	//		offset_y[i] = y + the_addition / 2;	// changes y coord
	//		offset_z[i] = z + the_addition;		// changes z coord

	//		(*this->modelStack).PushMatrix(); // render the hoops
	//		(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); // sets the coords of each hoop (coord stored in an array for each hoop)
	//		RenderMesh(this->meshGetFast("hoop"), true);
	//		(*this->modelStack).PopMatrix();
	//		the_addition += 20; // increases addition value so it keeps going

	//	}
	//}
	//
	//// earth -400, 0, -400, 21.0f, 21.0f, 21.0f

	//x = -350, y = 0, z = -350, the_addition = 15; // sets coords for next hoop range (near earth)

	//if (x = -350, y = 0, z = -350)
	//{
	//	for (int i = 5; i < 10; i++)
	//	{
	//		offset_x[i] = x + the_addition;
	//		offset_y[i] = y + the_addition;
	//		offset_z[i] = z + the_addition;

	//		(*this->modelStack).PushMatrix(); // render the hoops
	//		(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); 
	//		RenderMesh(this->meshGetFast("hoop"), true);
	//		(*this->modelStack).PopMatrix();
	//		the_addition += 25;
	//	}
	//}

	//// mars -550, 0, 550, 15.0f, 15.0f, 15.0f

	//x = -600, y = 10, z = 500, the_addition = 5; // sets coords for next hoop range (near mars)

	//if (x = -600, y = 10, z = 500)
	//{
	//	for (int i = 10; i < 15; i++)
	//	{
	//		offset_x[i] = x + the_addition;
	//		offset_y[i] = y + the_addition;
	//		offset_z[i] = z + the_addition;

	//		(*this->modelStack).PushMatrix(); // render the hoops
	//		(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]);
	//		RenderMesh(this->meshGetFast("hoop"), true);
	//		(*this->modelStack).PopMatrix();
	//		the_addition += 40;
	//	}
	//}

	//// jupiter 800, 0, 800, 75.0f, 75.0f, 75.0f

	//x = 900, y = 50, z = 850, the_addition = 20; // sets coords for next hoop range (near jupiter)

	//if (x = 900, y = 50, z = 850)
	//{
	//	for (int i = 15; i < 20; i++)
	//	{
	//		offset_x[i] = x + the_addition;
	//		offset_y[i] = y + the_addition;
	//		offset_z[i] = z + the_addition;

	//		(*this->modelStack).PushMatrix(); // render the hoops
	//		(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); //change coords accordingly (automate later)
	//		RenderMesh(this->meshGetFast("hoop"), true);
	//		(*this->modelStack).PopMatrix();
	//		the_addition += 15;
	//	}
	//}

	///////* end of hoops *///////

	this->RenderTextScreen(this->STData->font, this->dtimestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 2.f, 1.f, 15.f);
    std::map<std::string, entity*>::iterator it;

    for (it = this->entitylists->begin(); it != this->entitylists->end(); it++)
	{
		(*this->modelStack).PushMatrix();
		entity *buff = it->second;
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
			entity *spaceship = this->entityGetFast("spaceship1");

			// Matrix method v2
			(*this->modelStack).PushMatrix();
			cubeMult1.SetToTranslation(spaceship->position.x, spaceship->position.y, spaceship->position.z);

			if (Application::IsKeyPressed(VK_LEFT))
			{
				cubeMultR.SetToRotation(spaceship->angle, spaceship->up.x, spaceship->up.y, spaceship->up.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed(VK_RIGHT))
			{
				cubeMultR.SetToRotation(-(spaceship->angle), spaceship->up.x, spaceship->up.y, spaceship->up.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed(VK_UP))
			{
				cubeMultR.SetToRotation(-(spaceship->angle), spaceship->right.x, spaceship->right.y, spaceship->right.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed(VK_DOWN))
			{
				cubeMultR.SetToRotation(spaceship->angle, spaceship->right.x, spaceship->right.y, spaceship->right.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed('Q'))
			{
				cubeMultR.SetToRotation(-(spaceship->angle), spaceship->view.x, spaceship->view.y, spaceship->view.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (Application::IsKeyPressed('E'))
			{
				cubeMultR.SetToRotation(spaceship->angle, spaceship->view.x, spaceship->view.y, spaceship->view.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			cubeMult3.SetToScale(5.0f, 5.0f, 5.0f);
			cubeMatrix = cubeMult1 * cubeMult2 * cubeMult3;

			(*this->modelStack).LoadMatrix(cubeMatrix);
			RenderMesh(spaceship->meshptr, true);
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
		/*
        for (size_t i = 0; i < 8; i++)
        {
            (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(Ent2V[i].x, Ent2V[i].y, Ent2V[i].z);

            RenderMesh(this->meshGetFast("debugballs"), true);
            (*this->modelStack).PopMatrix();
        }
		*/
	}
}

void StateGame::OnCam(int X, int Y, float XChange, float YChange)
{
}