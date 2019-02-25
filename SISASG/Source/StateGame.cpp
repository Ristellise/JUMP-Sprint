#include "StateGame.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"
#include "genericEntity.h"
#include "Bullet.h"


StateGame::StateGame()
{
    this->StateName = "Game";
}


StateGame::~StateGame()
{
}

void StateGame::OnEnter()
{

	hoopGenerate(); // Generates the locations of the hoops based on the selected planet

    // Reset stats
    this->STData->moneyEarned = 0;
    this->STData->pointsPrev = 0;
    this->STData->timePrev = 0;

    // Camera reset
    this->state_cam->Reset();
    
    // Resets
    elapsedTime = 60;
    points = 0;

    // Matrix method
    cubeMatrix.SetToIdentity();
    cubeMultR.SetToIdentity();
    cubeMult1.SetToIdentity();
    cubeMult2.SetToIdentity();
    cubeMult3.SetToIdentity();

    Mesh* meshbuffer;

    meshbuffer = meshGetFast("spaceship");
    meshisnull = false;
    if (meshbuffer == nullptr)
    {
        meshisnull = true;
    }
    // Ship loading
    switch (this->STData->shipSelect)
    {
    case 0:
        // Ship1
        meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship1.obj")[0];
        meshbuffer->textureID = LoadTGA("TGA//Ship1.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 1:
        // Ship2
        meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship2.obj")[0];
        meshbuffer->textureID = LoadTGA("TGA//Ship2.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 2:
        // Ship3
        meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship3.obj")[0];
        meshbuffer->textureID = LoadTGA("TGA//Ship3.tga", GL_LINEAR, GL_CLAMP);
        break;
    }    
    if (meshisnull)
    {
        this->meshList->push_back(meshbuffer);
    }

    // Spaceship
    spaceship* spaceship1 = new spaceship();
    spaceship1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
    spaceship1->setWindow(this->winMan);
    spaceship1->type = entityType::eT_Ship;
    spaceship1->name = "spaceship";
    spaceship1->physics = true;
    spaceship1->Boxsize = BBoxDimensions(2.5f, 2.5f, 2.5f);
    spaceship1->meshptr = meshbuffer;

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
    spaceship1->Reset();
    this->entitylists->insert_or_assign("spaceship", spaceship1);
    

    meshbuffer = meshGetFast("planet");
    meshisnull = false;
    if (meshbuffer == nullptr)
    {
        meshisnull = true;
        meshbuffer = MeshBuilder::GenerateOBJ("planet", "OBJ//Planet sphere.obj")[0];
    }
    switch (this->STData->planetSelect)
    {
    case 0:
        // Venus
        meshbuffer->textureID = LoadTGA("TGA//venus texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 1:
        // Earth
        meshbuffer->textureID = LoadTGA("TGA//earth texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 2:
        // Mars
        meshbuffer->textureID = LoadTGA("TGA//mars texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 3:
        // Jupiter
        meshbuffer->textureID = LoadTGA("TGA//jupiter texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    }
    if (meshisnull)
    {
        this->meshList->push_back(meshbuffer);
    }

    // Test Env
    // meshbuffer = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];
    // meshbuffer->textureID = LoadTGA("TGA//TestEnv.tga", GL_LINEAR, GL_CLAMP);
    // this->meshList->push_back(meshbuffer);

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

	// Particle
	meshbuffer = MeshBuilder::GenerateQuad("particle", Color(0.f, 255.f, 255.f),1);
	meshbuffer->textureID = LoadTGA("TGA//testparticle.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Axes
	meshbuffer = MeshBuilder::GenerateAxes("axes", 200, 200, 200);
	this->meshList->push_back(meshbuffer);
    // Axes
    // meshbuffer = MeshBuilder::GenerateAxes("axes", 200, 200, 200);
    // this->meshList->push_back(meshbuffer);

	// Bullet
	meshbuffer = MeshBuilder::GenerateSphere("bullet", Color(255, 255, 255), 18, 36, 1);
	this->meshList->push_back(meshbuffer);

	Bullet* bullet = new Bullet();
	bullet->Init(Vector3(spaceship1->position.x,spaceship1->position.y,spaceship1->position.z), Vector3(spaceship1->target.x,spaceship1->target.y,spaceship1->target.z), Vector3(0, 1, 0));
	bullet->type = entityType::eT_Bullet;
	bullet->name = "bullet";
	bullet->size = (1.f, 1.f, 1.f);
	bullet->meshptr = this->meshGetFast("bullet");
	this->entitylists->insert_or_assign("bullet", bullet); 

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

    Stars();
    std::cout << this->entitylists->size() << std::endl;
    std::cout << this->meshList->size() << std::endl;
}

void StateGame::OnExit()
{
	delete this->entitylists->find("spaceship")->second;
	this->entitylists->erase("spaceship");
	while (hoopPos.size()) // Clears the hoops
	{
		hoopPos.pop_back();
	}
}

void StateGame::OnUpdate(double dt)
{
    elapsedTime -= dt;

    static int rotateDir = 1;
    static const float ROTATE_SPEED = 10.f;
    rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

    entity* spaceship = this->entityGetFast("spaceship");

    this->dgamestring = "POINTS OBTAINED: ";
    this->dgamestring += std::to_string(points);
    this->dgamestring += "\nTIME REMAINING: ";
    this->dgamestring += std::to_string(elapsedTime);

    this->dtimestring = "\n\nFPS: ";
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

    if ((points >= totalHoops) || (elapsedTime <= 0.0))
    {
		if (elapsedTime < 0.0)
		{
			elapsedTime = 0.0;
		}

        this->STData->moneyEarned = (unsigned long long)(points * elapsedTime);

        this->STData->pointsPrev = points;
        this->STData->timePrev = elapsedTime;
        this->STData->moneyData += this->STData->moneyEarned;
        this->readyExitlocal = true;
        this->spawnState = "Stat";
    }

    hoopChecker();
	Exhaust.GenerateParticles(dt);
	Exhaust.ParticleUpdate(dt);
	Exhaust.setplocation(spaceship->position);

    std::map<std::string, entity*>::iterator it;
    for (it = this->entitylists->begin(); it != this->entitylists->end(); it++)
    {
        it->second->Update(dt);
    } // Calling Updates.

    //spaceship->Update(dt);

    this->state_cam->Update(dt, *spaceship);

    this->collideInstance->updatingEnts = 0;

    this->collideInstance->doCollisions(*this->entitylists, dt);

    if (this->winMan->IsKeyPressed('R'))
    {
        this->readyExitlocal = true;
        this->spawnState = "Menus";
    }

}

void StateGame::hoopChecker()
{
    ///////* start of planet and hoop stuff *///////

    entity* spaceship = this->entityGetFast("spaceship");

    // generates the hoop checkers
    for (unsigned int i = 0; i < hoopPos.size(); i++) // for loop follows array
    {
        // passes values into hoops for coords
        if ((hoop.hoopsCheckXY((int)hoopPos[i].offset_x, (int)hoopPos[i].offset_y, (int)hoopPos[i].offset_z, (int)spaceship->position.x, (int)spaceship->position.y, (int)spaceship->position.z, (int)rad)) && hoopPos[i].passed == false)
        {
            points++;
            hoopPos[i].passed = true;
        }
    }

    ///////* end of planet and hoop stuff *///////
}

void StateGame::hoopRender()
{
    for (int i = 0; i < totalHoops; i++)
    {
        if (hoopPos[i].passed != true)
        {
            (*this->modelStack).PushMatrix(); // render the hoops
            (*this->modelStack).Translate(hoopPos[i].offset_x, hoopPos[i].offset_y, hoopPos[i].offset_z); // sets the coords of each hoop (coord stored in an array for each hoop)
            (*this->modelStack).Rotate(hoopPos[i].rotation, 1, 0, 0);
            RenderMesh(this->meshGetFast("hoop"), true);
            (*this->modelStack).PopMatrix();
        }
    }
}

void StateGame::hoopGenerate()
{

    // venus
    float the_addition = 10.f, the_subtraction = 0.f;

    if (ok.rotation == 360)
    {
        ok.rotation = 0;
    }

	switch (this->STData->planetSelect)
	{
	case(0): // venus
		z = 400.f;
		totalHoops = 5;
		for (int i = 0; i < totalHoops; i++)
		{
			hoopPos.push_back(ok);
			if (i > 2 && i < totalHoops)
			{
				the_subtraction -= 20;
				hoopPos[i].offset_x = x + the_subtraction * 2;
				hoopPos[i].offset_y = y + the_subtraction;
			}
			else
			{
				hoopPos[i].offset_x = x + the_addition * 2;
				hoopPos[i].offset_y = y + the_addition;
				the_subtraction += 20;
			}

            hoopPos[i].offset_z = z + the_addition * 3;

            the_addition += 30.f; // increases addition value so it keeps going

            if (i > 1)
                hoopPos[i].rotation += 90 + hoopPos[i - 1].rotation; // for rotation of hoops

		}
		break;
	case(1): // earth
		x = 100.f;
		z = 400.f;
		totalHoops = 6;
		for (int i = 0; i < totalHoops; i++)
		{
			hoopPos.push_back(ok);
			if (i > 2 && i < totalHoops)
			{
				the_subtraction -= 30;
				hoopPos[i].offset_x = x + the_subtraction * 2;
				hoopPos[i].offset_y = y + the_subtraction;
				hoopPos[i].offset_z = z + the_subtraction;
			}
			else
			{
				hoopPos[i].offset_x = x + the_addition;
				hoopPos[i].offset_y = y + the_addition * 2;
				hoopPos[i].offset_z = z + the_addition * 3;
				the_subtraction += 20;
			}

			the_addition += 50; // increases addition value so it keeps going
			if (i > 1)
				hoopPos[i].rotation += 90 + hoopPos[i - 1].rotation; // for rotation of hoops
		}
		break;
	case(2): // mars
		x = 100.f;
		z = 500.f;
		totalHoops = 7;

        for (int i = 0; i < totalHoops; i++)
        {
            hoopPos.push_back(ok);
            if (i > 2 && i < totalHoops)
            {
                the_subtraction -= 30.f;
                hoopPos[i].offset_x = x - the_subtraction * 1.5f;
                hoopPos[i].offset_y = y - the_subtraction;
            }
            else if (i == totalHoops)
            {
                hoopPos[i].offset_x = x - the_addition;
                hoopPos[i].offset_y = y + the_addition;
            }
            else
            {
                hoopPos[i].offset_x = x - the_addition * 2;
                hoopPos[i].offset_y = y + the_addition;
                the_subtraction += 50.f;
            }

			hoopPos[i].offset_z = z + the_addition * 3;
			the_addition += 40; // increases addition value so it keeps going
								//rotation += 90; // for rotation of hoops
		}
		break;
	case(3): // jupiter
		x = 200.f;
		z = 400.f;
		totalHoops = 8;
		for (int i = 0; i < totalHoops; i++)
		{
			hoopPos.push_back(ok);
			if (i > 2 && i < totalHoops)
			{
				the_subtraction -= 20;
				hoopPos[i].offset_x = x - the_subtraction * 1.5;
				hoopPos[i].offset_y = y - the_subtraction;
				hoopPos[i].offset_z = z + the_subtraction * 2;
			}
			else
			{
				hoopPos[i].offset_x = x + the_addition * 2;
				hoopPos[i].offset_y = y - the_addition;
				hoopPos[i].offset_z = z + the_addition * 3;
				the_subtraction += 15;
			}
			the_addition += 60; // increases addition value so it keeps going
								//rotation += 90; // for rotation of hoops
		}
		break;
	default:
		break;
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

	for (unsigned int i = 0; Exhaust.particles.size() > i; i++)
	{
		(*this->modelStack).PushMatrix();
		(*this->modelStack).Translate(Exhaust.particles[i].Position.x, Exhaust.particles[i].Position.y, Exhaust.particles[i].Position.z);
		(*this->modelStack).Rotate(this->entityGetFast("spaceship")->yawTotal, 0, 1, 0);
		(*this->modelStack).Rotate(this->entityGetFast("spaceship")->pitchTotal, 1, 0, 0);
		(*this->modelStack).Scale(1,1,1);		
		RenderMesh(this->meshGetFast("particle"), false);
		(*this->modelStack).PopMatrix();
	}

    // Planet
    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(400, 0, 1000);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(200.f, 200.f, 200.f);
    RenderMesh(this->meshGetFast("planet"), true);
    (*this->modelStack).PopMatrix();

    hoopRender();

    this->RenderTextScreen(this->STData->font, this->dgamestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 4.f, 1.f, 9.5f);
    this->RenderTextScreen(this->STData->font, this->dtimestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 2.f, 2.f, 20.f);

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
                buff->position.z,    // Used for Text Scaling. only applies to 2d UI 
                buff->position.x,    // Same as before
                buff->position.y);    // Same as before
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
		else if (buff->type == entityType::eT_Bullet)
		{
			// spaceship spaceship1;
			// (*this->modelStack).PushMatrix();
			// (*this->modelStack).Translate(0, 0, 0);
			// RenderMesh(buff->meshptr, true);
			// (*this->modelStack).PopMatrix();
		}
		(*this->modelStack).PopMatrix();

        // (*this->modelStack).PushMatrix();
        // (*this->modelStack).Translate(0, 0, 0);
        // RenderMesh(this->meshGetFast("axes"), false);
        // (*this->modelStack).PopMatrix();
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
