#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"
#include "Bullet.h"

void Stateinit::OnEnter()
{
	this->spawnState = "Title";

	// Init Stacks
	std::cout << "Entering: " << this->StateName << " Albion Prelude." << std::endl;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	this->projectionStack->LoadMatrix(projection);

	// Load meshes first.
	Mesh* meshbuffer;
	meshbuffer = MeshBuilder::GenerateText("saofontsheet", *this->St_FLInstance);
	meshbuffer->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);
	this->meshList->push_back(meshbuffer);

	meshbuffer = MeshBuilder::GenerateSphere("debugballs", Color(1, 1, 1), 10, 10, 0.25f);
	this->meshList->push_back(meshbuffer);

	meshbuffer = MeshBuilder::GenerateSphere("bullet", Color(1, 1, 1), 18, 36, 0.5);
	this->meshList->push_back(meshbuffer);

	// Camera
	this->state_cam->Init(Vector3(0, 4, -40), Vector3(0, 4, 1), Vector3(0, 1, 0));

	// Spawn Entities.
	entity* current = new entity();

	// Debugging string
	// Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
	current->Init(Vector3(1.f, 24.f, 2.f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	current->text = &dtimestring;
	current->type = entityType::eT_TextUI;
	current->meshptr = this->meshGetFast("saofontsheet");
	this->entitylists->insert_or_assign("saofontsheet", current);
}

void Stateinit::OnRender()
{

}

void Stateinit::OnCam(int X, int Y, float XChange, float YChange)
{
}

void Stateinit::OnExit()
{
	std::cout << "Shutdown: " << this->StateName << std::endl;
}

Stateinit::Stateinit()
{
	this->StateName = "init";
}

void Stateinit::OnUpdate(double dt)
{
	spaceship* spaceship1 = new spaceship();
	if (this->STData->bounceTime >= 0.0)
	{
		this->STData->bounceTime -= dt;
	}

	//bullet->timeAlive += dt;
	//spaceship1->Update(dt);

	//bullet->Update(dt); // Calls Bullet:Update();

	/*if (bullet->timeAlive > 1)
	if (this->STData->bounceTime >= 0.0)
	{
		Bullet* bullet = new Bullet(Vector3(spaceship1->position.x, spaceship1->position.y, spaceship1->position.z),
			Vector3(spaceship1->target.x, spaceship1->target.y, spaceship1->target.z),
			Vector3(0, 1, 0));
		delete bullet;
		this->bullet->timeAlive = 0;
		this->STData->bounceTime -= dt;
	}*/
}