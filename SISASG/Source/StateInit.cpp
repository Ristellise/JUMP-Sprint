#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"

void Stateinit::OnEnter()
{
	this->spawnState = "title";

	// Init Stacks
    std::cout << "Entering: " << this->StateName<< " Albion Prelude."<< std::endl;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	this->projectionStack->LoadMatrix(projection);

    // Load meshes first.
    Mesh* meshbuffer;
    meshbuffer = MeshBuilder::GenerateText("saofontsheet", *this->St_FLInstance);
    meshbuffer->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);
    this->meshList->push_back(meshbuffer);

    meshbuffer = MeshBuilder::GenerateSphere("debugballs",Color(1,1,1),10,10,0.5f);
    this->meshList->push_back(meshbuffer);

    // Camera
    this->state_cam->Init(Vector3(0, 4, -30), Vector3(0, 4, 1), Vector3(0, 1, 0));
    // Spawn Entities.
    entity* current = new entity();

	// Debugging string
	// Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
    current->text = &dtimestring;
    current->type = entityType::eT_TextUI;
    current->meshptr = this->meshGetFast("saofontsheet");
    this->entitylists->push_back(current);

    // Collision tester
	/*
    current = new entity();

    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(0, 0, 1), Vector3(0, 1, 0) );
    current->type = entityType::eT_Object;
    current->meshptr = this->meshGetFast("testcube");
    current->physics = true;
    current->Boxsize = BBoxDimensions(3.0f, 3.0f, 3.0f);
    this->entitylists->push_back(current);
	*/
}

void Stateinit::OnRender()
{
	;
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
	if (*bounceTime >= 0.0)
	{
		*bounceTime -= dt;
	}
}
