#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
/*-----------------
- StateInit:
-- First code to actually run. Everything starts from here.
*/

void Stateinit::OnEnter()
{
    // Init Stacks

    std::cout << "Entering: " << this->StateName<< " Albion Prelude."<< std::endl;

	// this->state_cam->Init(Vector3(0, 10, -30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	this->projectionStack->LoadMatrix(projection);

    // Load meshes first.
    Mesh* meshbuffer;
    meshbuffer = MeshBuilder::GenerateText("saofontsheet", *this->St_FLInstance);
    meshbuffer->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);
    this->meshList.push_back(meshbuffer);

	meshbuffer = MeshBuilder::GenerateOBJ("testcube", "OBJ//TestCube.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestCube.tga", GL_LINEAR, GL_CLAMP);
	this->meshList.push_back(meshbuffer);

    // Spawn Entities.
    entity* current = new entity();

	this->state_cam->Init(Vector3(0, 4, -30), Vector3(0, 4, 1), Vector3(0, 1, 0));

	// Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
    current->Init(Vector3(1, 15.0f, 3), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
    current->text = &dtimestring;
    current->type = entityType::eT_TextUI;
    current->meshptr = this->meshGetFast("saofontsheet");
    this->entitylists.push_back(current);

    current = new entity();

    current->Init(Vector3(1, 2.0f, 5.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
    *current->text = "Hit me Senpai~";
    current->type = entityType::eT_Text;
    current->meshptr = this->meshGetFast("saofontsheet");
    this->entitylists.push_back(current);

	// Test Cube
	testCube* testCube1 = new testCube();
	testCube1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	testCube1->type = entityType::eT_Space;
	testCube1->name = "testcube";
	testCube1->meshptr = this->meshGetFast("spaceship");
	this->entitylists.push_back(testCube1);
}

void Stateinit::OnRender()
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

	entity* testCube1 = this->entityGetFast("testcube");

	testCube1->Update(dt);
    this->state_cam->Update(dt, *testCube1);

}
