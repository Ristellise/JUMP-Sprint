#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"

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
    this->meshList->push_back(meshbuffer);

	meshbuffer = MeshBuilder::GenerateOBJ("testcube", "OBJ//TestCube.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//TestCube.tga", GL_LINEAR, GL_CLAMP);
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
    current = new entity();

    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(0, 0, 1), Vector3(0, 1, 0) );
    current->type = entityType::eT_Object;
    current->meshptr = this->meshGetFast("testcube");
    current->physics = true;
    current->Boxsize = BBoxDimensions(3.0f, 3.0f, 3.0f);
    this->entitylists->push_back(current);

	// Test Cube
	testCube* testCube1 = new testCube();
	testCube1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	testCube1->type = entityType::eT_Space;
	testCube1->name = "testcube";
    testCube1->physics = true;
    testCube1->Boxsize = BBoxDimensions(2.5f, 2.5f, 2.5f);
	testCube1->meshptr = this->meshGetFast("testcube");
	this->entitylists->push_back(testCube1);

	// Matrix method
	cubeMatrix.SetToIdentity();
	cubeMultR.SetToIdentity();
	cubeMult1.SetToIdentity();
	cubeMult2.SetToIdentity();
	cubeMult3.SetToIdentity();
}

void Stateinit::OnRender()
{
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
		else if (buff->type == entityType::eT_Space)
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

	if ((Application::IsKeyPressed('5')) && *debugToggle == false && *bounceTime <= 0.0)
	{
		*debugToggle = true;
		*bounceTime = 0.3;
		this->spawnState = "debugger";
	}

	entity* testCube1 = this->entityGetFast("testcube");

	testCube1->Update(dt);

	this->state_cam->Update(dt, *testCube1);

	this->collideInstance->updatingEnts = 0;

    this->collideInstance->doCollisions(*this->entitylists);

    
    

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
