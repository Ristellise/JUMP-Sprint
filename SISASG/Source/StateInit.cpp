#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"


void Stateinit::OnEnter()
{
    // Init Stacks

    std::cout << "Entering: " << this->StateName<< " Albion Prelude."<< std::endl;

	this->state_cam->Init(Vector3(0, 10, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));
	

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

	this->state_cam->Init(Vector3(0, 10, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
    current->Init(Vector3(1, 1, 2), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
    current->text = &dtimestring;
    current->type = entityType::eT_TextUI;
    current->meshptr = this->meshGetFast("saofontsheet");
    this->entitylists.push_back(current);

	// Test Cube
	testCube* testCube1 = new testCube();
	testCube1->Init(Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0));
	testCube1->type = entityType::eT_Space;
	testCube1->name = "testcube";
	testCube1->meshptr = this->meshGetFast("testcube");
	this->entitylists.push_back(testCube1);
}

void Stateinit::OnRender()
{
    for (size_t i = 0; i < this->entitylists.size(); i++)
    {
        
        (*this->modelStack).PushMatrix();
        
        if (this->entitylists[i]->type == entityType::eT_Text)
        {
            this->entitylists[i]->position;
            (*this->modelStack).Translate(this->entitylists[i]->position.x,
                                          this->entitylists[i]->position.y,
                                          this->entitylists[i]->position.z);
            // rotation coords
            this->RenderText(this->entitylists[i]->meshptr, *this->entitylists[i]->text, Color(0, 0, 0));
        }
        else if (this->entitylists[i]->type == entityType::eT_TextUI)
        {
            this->RenderTextScreen(this->entitylists[i]->meshptr, *this->entitylists[i]->text, Color(0, 0, 0), 
                                   this->entitylists[i]->position.z,	// Used for Text SCaling. only applies to 2d UI 
                                   this->entitylists[i]->position.x,	// Same as before
                                   this->entitylists[i]->position.y);	// Same as before
        }
		else if (this->entitylists[i]->type == entityType::eT_Space)
		{
			(*this->modelStack).Translate(this->entitylists[i]->position.x, this->entitylists[i]->position.y, this->entitylists[i]->position.z);
			(*this->modelStack).Rotate(this->entitylists[i]->yawTotal, 0, 1, 0);
			(*this->modelStack).Rotate(this->entitylists[i]->pitchTotal, 1, 0, 0);
			(*this->modelStack).Scale(1.0f, 1.0f, 1.0f);
			RenderMesh(this->entitylists[i]->meshptr, true);
		}
        (*this->modelStack).PopMatrix();
    }

	// Test Cube
	
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
	this->state_cam->Update(
		dt,
		testCube1->position.x,
		testCube1->position.y,
		testCube1->position.z,
		testCube1->topSpeed,
		testCube1->fwdaccl,
		testCube1->bwdaccl,
		state_cam->accl,
		testCube1->view
	);

	testCube1->Update(dt,
		testCube1->topSpeed,
		testCube1->fwdaccl,
		testCube1->bwdaccl,
		testCube1->accl);

	this->dtimestring = "FPS:";
	this->dtimestring += std::to_string(1.0f / dt);
	this->dtimestring += "\nCam X:";
	this->dtimestring += std::to_string(this->state_cam->position.x);
	this->dtimestring += "\nCam Y:";
	this->dtimestring += std::to_string(this->state_cam->position.y);
	this->dtimestring += "\nCam Z:";
	this->dtimestring += std::to_string(this->state_cam->position.z);
	this->dtimestring += "\nVel :";
	this->dtimestring += std::to_string(testCube1->velocity);
	this->dtimestring += "\nAcl :";
	this->dtimestring += std::to_string(testCube1->accl);
	this->dtimestring += "\nPit :";
	this->dtimestring += std::to_string(testCube1->pitchTotal);
	this->dtimestring += "\nYaw :";
	this->dtimestring += std::to_string(testCube1->yawTotal);

	this->dtimestring += "\nCamVel :";
	this->dtimestring += std::to_string(state_cam->velocity);
	this->dtimestring += "\nCamAcl :";
	this->dtimestring += std::to_string(state_cam->accl);
}
