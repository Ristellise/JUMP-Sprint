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

	// Camera
	this->state_cam->Init(Vector3(0, 4, -30), Vector3(0, 4, 1), Vector3(0, 1, 0));

	// Debugging string
	// Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
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

	// Matrix method
	cubeMatrix.SetToIdentity();
	cubeMultR.SetToIdentity();
	cubeMult1.SetToIdentity();
	cubeMult2.SetToIdentity();
	cubeMult3.SetToIdentity();
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
			/*
			(*this->modelStack).Translate(this->entitylists[i]->position.x, this->entitylists[i]->position.y, this->entitylists[i]->position.z);
			(*this->modelStack).Rotate(this->entitylists[i]->yawTotal, 0, 1, 0);
			(*this->modelStack).Rotate(this->entitylists[i]->pitchTotal, 1, 0, 0);
			(*this->modelStack).Scale(1.0f, 1.0f, 1.0f);
			RenderMesh(this->entitylists[i]->meshptr, true);
			*/

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
        (*this->modelStack).PopMatrix();
    }
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
	this->dtimestring += "\nRol :";
	this->dtimestring += std::to_string(testCube1->rollTotal);

    this->dtimestring += "\nMouse:" + std::to_string(this->mouse->X) +
        " | " + std::to_string(this->mouse->Y) +
        " | Change: " + std::to_string(this->mouse->XChange) +
        " | " + std::to_string(this->mouse->YChange);

	if (Application::IsKeyPressed('R'))
	{
		// Matrix method
		cubeMatrix.SetToIdentity();
		cubeMultR.SetToIdentity();
		cubeMult1.SetToIdentity();
		cubeMult2.SetToIdentity();
		cubeMult3.SetToIdentity();
	}
}
