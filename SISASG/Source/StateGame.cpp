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
	// Reset stats
	this->STData->moneyEarned = 0;
	this->STData->pointsPrev = 0;
	this->STData->timePrev = 0;

	// Camera reset
	this->state_cam->Reset();
	
	// Elapsed time
	elapsedTime = 60;

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

	switch (this->STData->planetSelect)
	{
	case 0:
		// Venus
		meshbuffer = MeshBuilder::GenerateOBJ("planet", "OBJ//Planet sphere.obj")[0];
		meshbuffer->textureID = LoadTGA("TGA//venus texture.tga", GL_LINEAR, GL_CLAMP);
		break;
	case 1:
		// Earth
		meshbuffer = MeshBuilder::GenerateOBJ("planet", "OBJ//Planet sphere.obj")[0];
		meshbuffer->textureID = LoadTGA("TGA//earth texture.tga", GL_LINEAR, GL_CLAMP);
		break;
	case 2:
		// Mars
		meshbuffer = MeshBuilder::GenerateOBJ("planet", "OBJ//Planet sphere.obj")[0];
		meshbuffer->textureID = LoadTGA("TGA//mars texture.tga", GL_LINEAR, GL_CLAMP);
		break;
	case 3:
		// Jupiter
		meshbuffer = MeshBuilder::GenerateOBJ("planet", "OBJ//Planet sphere.obj")[0];
		meshbuffer->textureID = LoadTGA("TGA//jupiter texture.tga", GL_LINEAR, GL_CLAMP);
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
	
	switch (this->STData->shipSelect)
	{
	case 0:
		spaceship1->topSpeed = 40.0f;
		spaceship1->fwdaccl = 20.f;
		spaceship1->bwdaccl = -20.f;
		spaceship1->drift = 7.0f;
		break;
	case 1:
		spaceship1->topSpeed = 60.0f;
		spaceship1->fwdaccl = 15.f;
		spaceship1->bwdaccl = -15.f;
		spaceship1->drift = 10.f;
		break;
	case 2:
		spaceship1->topSpeed = 80.0f;
		spaceship1->fwdaccl = 10.f;
		spaceship1->bwdaccl = -10.f;
		spaceship1->drift = 5.0f;
		break;
	}
	
	this->entitylists->insert_or_assign("spaceship", spaceship1);

	// Test Env
	meshbuffer = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestEnv.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Audio src
    // this->audiosrc.Load("Audio/testtrack.flac");

	/*
	// Test Env
	entity* testEnv = new entity();
	testEnv->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	testEnv->type = entityType::eT_Environment;
	testEnv->name = "testenv";
	// testEnv->physics = true;
	// testEnv->Boxsize = BBoxDimensions(0.f, 0.f, 0.f);
	testEnv->meshptr = this->meshGetFast("testenv");
	this->entitylists->insert_or_assign("testenv",testEnv);
	*/

	// Hoops
	meshbuffer = MeshBuilder::GenerateTorus("hoop", Color(0.f, 255.f, 255.f), 36, 36, 15, 1);
	this->meshList->push_back(meshbuffer);

	// Axes
	meshbuffer = MeshBuilder::GenerateAxes("axes", 200, 200, 200);
	this->meshList->push_back(meshbuffer);


    // Collision tester
	/*
    entity* current = new genericEntity();
    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(0, 0, 1), Vector3(0, 1, 0));
    current->type = entityType::eT_Object;
    current->meshptr = this->meshGetFast("spaceship");
    current->physics = true;
    current->Boxsize = BBoxDimensions(0.5f, 0.5f, 0.5f);
    this->entitylists->insert_or_assign("testcube",current);
	*/

    //this->STData->VERYLOUD.play();

	Stars();

	for (int i = 0; i < 20; i++)
	{
		idk.push_back(ok);
	}
    this->STData->SoundSrcs["looptest"]->enableLooping();
    this->STData->SoundSrcs["looptest"]->pause();
}

void StateGame::OnExit()
{
}

void StateGame::OnUpdate(double dt)
{
	elapsedTime -= dt;

	static int rotateDir = 1;
	static const float ROTATE_SPEED = 10.f;
	rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

	entity* spaceship = this->entityGetFast("spaceship");

	this->dtimestring = "POINTS OBTAINED: ";
	this->dtimestring += std::to_string(points);
	this->dtimestring += "\nTIME REMAINING: ";
	this->dtimestring += std::to_string(elapsedTime);

	this->dtimestring += "\n\nFPS: ";
	this->dtimestring += std::to_string(1.0f / dt);
	this->dtimestring += "\nCAM X: ";
	this->dtimestring += std::to_string(this->state_cam->position.x);
	this->dtimestring += "\nCAM Y: ";
	this->dtimestring += std::to_string(this->state_cam->position.y);
	this->dtimestring += "\nCAM Z: ";
	this->dtimestring += std::to_string(this->state_cam->position.z);

	this->dtimestring += "\nVEL: ";
	this->dtimestring += std::to_string(spaceship->velocity);
	this->dtimestring += "\nACL: ";
	this->dtimestring += std::to_string(spaceship->accl);
	this->dtimestring += "\nPIT: ";				 
	this->dtimestring += std::to_string(spaceship->pitchTotal);
	this->dtimestring += "\nYAW: ";				 
	this->dtimestring += std::to_string(spaceship->yawTotal);
	this->dtimestring += "\nROL: ";				 
	this->dtimestring += std::to_string(spaceship->rollTotal);

	if ((points >= 5) || (elapsedTime <= 0))
	{
		this->STData->moneyEarned = (unsigned long long)(points * elapsedTime);
		this->STData->pointsPrev = points;
		this->STData->timePrev = elapsedTime;
		this->STData->moneyData += this->STData->moneyEarned;
		this->readyExitlocal = true;
		this->spawnState = "Stat";
	}

	///////* start of planet and hoop stuff *///////

	//test range coords for planet range (center sphere coords) (automate later)

	//float cx_venus = 300.f, cy_venus = 0.f, cz_venus = -300.f; // can manually set planet coords due to generate ones already
	//float cx_earth = -400.f, cy_earth = 0.f, cz_earth = -400.f;
	//float cx_mars = -550.f, cy_mars = 0.f, cz_mars = 550.f;
	//float cx_jupiter = 800.f, cy_jupiter = 0.f, cz_jupiter = 800.f;

	// checks whether planet and character is in range (venus)

	/*

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
	for (int i = 0; i < idk.size(); i++) // for loop follows array
	{
		// passes values into hoops for coords

		if ((hoop.hoopsExecuteUI((int)idk[i].offset_x, (int)idk[i].offset_y, (int)idk[i].offset_z, (int)spaceship->position.x, (int)spaceship->position.y, (int)spaceship->position.z, (int)rad)) && idk[i].passed == false)
		{
			points++;
			idk[i].passed = true;
			//hoop.hoopsExecuteUI((int)offset_x[i], (int)offset_y[i], (int)offset_z[i], (int)spaceship->position.x, (int)spaceship->position.y, (int)spaceship->position.z, (int)rad) == false;
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

	/*
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
	*/
}

void StateGame::hoopGenerate()
{
	// venus
	int the_addition = 10, the_subtraction = 0;

	if (ok.rotation == 360)
	{
		ok.rotation = 0;
	}

	if (this->STData->planetSelect == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			z = 400.f;

			if (i > 2 && i < 5)
			{
				the_subtraction -= 20;
				idk[i].offset_x = x + the_subtraction * 2;
				idk[i].offset_y = y + the_subtraction;
			}
			else
			{
				idk[i].offset_x = x + the_addition * 2;
				idk[i].offset_y = y + the_addition;
				the_subtraction += 20;
			}

			idk[i].offset_z = z + the_addition * 3;

			the_addition += 30; // increases addition value so it keeps going
			//rotation += 90; // for rotation of hoops
		}
	}

	if (this->STData->planetSelect == 1)
	{
		x = 100.f;
		z = 400.f;

		for (int i = 5; i < 10; i++)
		{

			if (i > 7 && i < 10)
			{
				the_subtraction -= 30;
				idk[i].offset_x = x + the_subtraction * 2;
				idk[i].offset_y = y + the_subtraction;
				idk[i].offset_z = z + the_subtraction;
			}
			else
			{
				idk[i].offset_x = x + the_addition;
				idk[i].offset_y = y + the_addition * 2;
				idk[i].offset_z = z + the_addition * 3;
				the_subtraction += 20;
			}

			the_addition += 50; // increases addition value so it keeps going
			ok.rotation += 90; // for rotation of hoops
		}
	}

	if (this->STData->planetSelect == 2)
	{
		x = 100.f;
		z = 400.f;

		for (int i = 10; i < 15; i++)
		{

			if (i > 12 && i < 15)
			{
				the_subtraction -= 30;
				idk[i].offset_x = x - the_subtraction;
				idk[i].offset_y = y + the_addition;
			}
			else
			{
				idk[i].offset_x = x - the_addition;
				idk[i].offset_y = y + the_addition * 2;
				the_subtraction += 50;
			}

			idk[i].offset_z = z + the_addition;

			the_addition += 40; // increases addition value so it keeps going
			//rotation += 90; // for rotation of hoops
		}
	}

	if (this->STData->planetSelect == 3)
	{
		x = 200.f;
		z = 400.f;

		for (int i = 15; i < 20; i++)
		{

			if (i > 17 && i < 20)
			{
				the_subtraction -= 50;
				idk[i].offset_x = x - the_subtraction;
				idk[i].offset_y = y - the_subtraction;
				idk[i].offset_z = z + the_subtraction * 3;
			}
			else
			{
				idk[i].offset_x = x + the_addition * 2;
				idk[i].offset_y = y - the_addition;
				idk[i].offset_z = z + the_addition * 3;
				the_subtraction += 15;
			}

			the_addition += 60; // increases addition value so it keeps going
			//rotation += 90; // for rotation of hoops
		}
	}

	for (int i = 0; i < 20; i++)
	{
		(*this->modelStack).PushMatrix(); // render the hoops
		(*this->modelStack).Translate(idk[i].offset_x, idk[i].offset_y, idk[i].offset_z); // sets the coords of each hoop (coord stored in an array for each hoop)
		//(*this->modelStack).Rotate(ok.rotation, 1, 0, 0);
		RenderMesh(this->meshGetFast("hoop"), true);
		(*this->modelStack).PopMatrix();
	}
}

void StateGame::OnRender()
{
	// Stars
	for (int i = 0; starsnumber > i; i++)
	{
		(*this->modelStack).PushMatrix();
		(*this->modelStack).Translate(stars[i].x, stars[i].y, stars[i].z);
		if (stars[i].stime == 0)
		{
			stars[i].scale = (abs(stars[i].x) + abs(stars[i].y) + abs(stars[i].z)) / ((rand() % 250) + 250); // Scale (Rand adds twinkles)
			stars[i].stime = rand() % 10 + 10;
		}
		(*this->modelStack).Scale(stars[i].scale, stars[i].scale, stars[i].scale);
		stars[i].stime--;
		RenderMesh(this->meshGetFast("star"), false);
		(*this->modelStack).PopMatrix();
	}

	// Planet
	(*this->modelStack).PushMatrix();
	(*this->modelStack).Translate(400, 0, 1000);
	(*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
	(*this->modelStack).Scale(200.f, 200.f, 200.f);
	RenderMesh(this->meshGetFast("planet"), true);
	(*this->modelStack).PopMatrix();

	///////* start of hoops *///////
	hoopGenerate();
	/*
	for loop to create 5 hoops?

	venus 300, 0, -300, 20.0f, 20.0f, 20.0f

	if (x = 250, y = 0, z = -250) // this double checks if specified start coordinates are right
	{
		for (int i = 0; i < 5; i++) // for loop follows amount of rings wanted inside the "map" (e.g 0 to 4 for this case, thus 5 hoops)
		{
			offset_x[i] = x + the_addition * 2;	// changes x coord (can multiply / divide all these to make it more spaced out)
			offset_y[i] = y + the_addition / 2;	// changes y coord
			offset_z[i] = z + the_addition;		// changes z coord

			(*this->modelStack).PushMatrix(); // render the hoops
			(*this->modelStack).Translate(offset_x[i], offset_y[i], offset_z[i]); // sets the coords of each hoop (coord stored in an array for each hoop)
			RenderMesh(this->meshGetFast("hoop"), true);
			(*this->modelStack).PopMatrix();
			the_addition += 20; // increases addition value so it keeps going

		}
	}
	
	earth -400, 0, -400, 21.0f, 21.0f, 21.0f

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

	mars -550, 0, 550, 15.0f, 15.0f, 15.0f

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

	jupiter 800, 0, 800, 75.0f, 75.0f, 75.0f

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
	*/
	///////* end of hoops *///////

	this->RenderTextScreen(this->STData->font, this->dtimestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 2.f, 1.f, 24.f);

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
			entity *spaceship = this->entityGetFast("spaceship");

			// Matrix method v2
			(*this->modelStack).PushMatrix();
			cubeMult1.SetToTranslation(spaceship->position.x, spaceship->position.y, spaceship->position.z);

			if (spaceship->lKey == true)
			{
				cubeMultR.SetToRotation(spaceship->angle, spaceship->up.x, spaceship->up.y, spaceship->up.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (spaceship->rKey == true)
			{
				cubeMultR.SetToRotation(-(spaceship->angle), spaceship->up.x, spaceship->up.y, spaceship->up.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (spaceship->uKey == true)
			{
				cubeMultR.SetToRotation(-(spaceship->angle), spaceship->right.x, spaceship->right.y, spaceship->right.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (spaceship->dKey == true)
			{
				cubeMultR.SetToRotation(spaceship->angle, spaceship->right.x, spaceship->right.y, spaceship->right.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (spaceship->qKey == true)
			{
				cubeMultR.SetToRotation(-(spaceship->angle), spaceship->view.x, spaceship->view.y, spaceship->view.z);
				cubeMult2 = cubeMultR * cubeMult2;
			}

			if (spaceship->eKey == true)
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

void StateGame::Stars()
{
	starsnumber = 200; // Generated number of stars
	for (int i = 0; starsnumber > i; i++)
	{
		stars.push_back(coord);
		float u = ((float)rand() / (RAND_MAX)) + 0.f;
		float v = ((float)rand() / (RAND_MAX)) + 0.f;
		float theta = 2 * Math::PI * u;
		float phi = acos(2 * v - 1);
		stars[i].x = 100.f + ((10000.f * 0.9f) * sin(phi) * cos(theta));
		stars[i].y = 100.f + ((10000.f * 0.9f) * sin(phi) * sin(theta));
		stars[i].z = 100.f + ((10000.f * 0.9f) * cos(phi));
	}
}