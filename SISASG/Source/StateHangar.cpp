#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"

#include "spaceship.h"
#include "StateHangar.h"

StateHangar::StateHangar()
{
	this->StateName = "Hangar";
}

void StateHangar::OnEnter()
{
	Mesh* meshbuffer;

	this->state_cam->Init(Vector3(0, 4, -40), Vector3(0, 4, 1), Vector3(0, 1, 0));

	// Ship 1
	meshbuffer = MeshBuilder::GenerateOBJ("ship1", "OBJ//Ship1.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//Ship1.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Ship 1
	entity* ship1 = new entity();
	ship1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	ship1->type = entityType::eT_Object;
	ship1->name = "ship1";
	ship1->size = Vector3(5.f, 5.f, 5.f);
	ship1->meshptr = this->meshGetFast("ship1");
	this->entitylists->insert_or_assign("ship1",ship1);

	// Ship 2
	meshbuffer = MeshBuilder::GenerateOBJ("ship2", "OBJ//Ship2.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//Ship2.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Ship 2
	entity* ship2 = new entity();
	ship2->Init(Vector3(-20, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	ship2->type = entityType::eT_Object;
	ship2->name = "ship2";
	ship2->size = Vector3(5.f, 5.f, 5.f);
	ship2->meshptr = this->meshGetFast("ship2");
	this->entitylists->insert_or_assign("ship2", ship2);

	// Side
	meshbuffer = MeshBuilder::GenerateQuad("sides", Color(0, 0, 0), 1);
	meshbuffer->textureID = LoadTGA("TGA//hangarsides.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

	// Star
	meshbuffer = MeshBuilder::GenerateSphere("star", Color(1, 1, 1), 2, 4, 1);
	this->meshList->push_back(meshbuffer);

	Stars();
}

void StateHangar::OnExit()
{
}

void StateHangar::OnUpdate(double dt)
{
	static const float LSPEED = 10.0f;
	static const float CSHIFT = 20.f;

	//For movement
	if ((Application::IsKeyPressed('A') || 
		((Application::IsKeyPressed(MK_LBUTTON)) && Dir == 1)) && 
		(state_cam->position.x < 0) && Delay == 0)
	{
		this->STData->shipSelect -= 1;
		Delay += 10;
		Shift = CSHIFT / Delay;
		shiftmovement = true;
	}

	if ((Application::IsKeyPressed('D') || 
		((Application::IsKeyPressed(MK_LBUTTON)) && Dir == -1)) && 
		(state_cam->position.x > (-CSHIFT * (NumberOfShips - 1)) && Delay == 0))
	{
		this->STData->shipSelect += 1;
		Delay += 10;
		Shift = -CSHIFT / Delay;
		shiftmovement = true;
	}

	if (Delay > 0) // Handles movement
	{
		if (shiftmovement)
		{
			state_cam->position.x += Shift;
			state_cam->target.x += Shift;
		}
		Delay--;
	}
	else
	{
		shiftmovement = false;
	}

	/*
	if (Application::IsKeyPressed(VK_SPACE) && Delay == 0)
	{
		shiftmovement = false;
		if (lit == true)
		{
			lit = false;
			lights[0].power = 0;
		}
		else
		{
			lit = true;
			lights[0].power = 5.f;
		}
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
		Delay += 10;
	}
	*/
}

void StateHangar::OnRender()
{
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

	RenderShips();
	RenderUI();
}

void StateHangar::OnCam(int X, int Y, float XChange, float YChange)
{
}

void StateHangar::RenderShips()
{
    std::map<std::string, entity*>::iterator it;

    for (it = this->entitylists->begin(); it != this->entitylists->end(); it++)
	{
		entity *buff = it->second;

		(*this->modelStack).PushMatrix();

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
		(*this->modelStack).PopMatrix();
	}
}

void StateHangar::RenderUI()
{
	(*this->modelStack).PushMatrix();
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	float sideDivision = 10.f;
	Dir = 0;

	if ((this->mouse->X < (this->mouse->winWidth / sideDivision)) || (this->mouse->X > (this->mouse->winWidth - (this->mouse->winWidth / sideDivision))))
	{
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI (left,right,bottom,top)
		(*this->projectionStack).PushMatrix();
		(*this->projectionStack).LoadMatrix(ortho);
		(*this->viewStack).PushMatrix();
		(*this->viewStack).LoadIdentity(); //No need camera for ortho mode
		(*this->modelStack).PushMatrix();
		(*this->modelStack).LoadIdentity(); //Reset modelStack
		if (this->mouse->X < (this->mouse->winWidth / sideDivision))
		{
			(*this->modelStack).Translate(80.f / sideDivision / 2.f, 30.f, 0.f);
			(*this->modelStack).Scale(80.f / sideDivision, 30.f, 1.f);
			Dir = 1;
		}
		else if (this->mouse->X > (this->mouse->winWidth - (this->mouse->winWidth / sideDivision)))
		{
			(*this->modelStack).Translate(80.f - (80.f / sideDivision / 2.f), 30.f, 0.f);
			(*this->modelStack).Scale(80.f / sideDivision, 30.f, 1.f);
			(*this->modelStack).Rotate(180.f, 0.f, 1.f, 0.f);
			Dir = -1;
		}
		RenderMesh(this->meshGetFast("sides"), false);
		(*this->projectionStack).PopMatrix();
		(*this->viewStack).PopMatrix();
		(*this->modelStack).PopMatrix();
	}
	else if (Application::IsKeyPressed(VK_LBUTTON) && this->STData->bounceTime <= 0.0)
	{
		this->STData->bounceTime = 0.3;
		this->readyExitlocal = true;
		this->spawnState = "Game";
	}

	glEnable(GL_DEPTH_TEST);
	(*this->modelStack).PopMatrix();
}

void StateHangar::Stars()
{
	starsnumber = 400; // Generated number of stars
	for (int i = 0; starsnumber > i; i++)
	{
		stars.push_back(coord);
		float u = ((float)rand() / (RAND_MAX)) + 0.f;
		float v = ((float)rand() / (RAND_MAX)) + 0.f;
		float theta = 2 * Math::PI * u;
		float phi = acos(2 * v - 1);
		stars[i].x = 100 + ((10000.f * 0.9) * sin(phi) * cos(theta));
		stars[i].y = 100 + ((10000.f * 0.9) * sin(phi) * sin(theta));
		stars[i].z = 100 + ((10000.f * 0.9) * cos(phi));
	}
}

StateHangar::~StateHangar()
{
}
