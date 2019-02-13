#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"


void Stateinit::OnEnter()
{
    // Init Stacks
    this->modelStack = new MS();
    this->viewStack = new MS();
    this->projectionStack = new MS();

    std::cout << "Entering: " << this->StateName<< " Albion Prelude."<< std::endl;
    
    // Load meshes first.
    Mesh* meshbuffer;
    meshbuffer = MeshBuilder::GenerateText("saofontsheet", *this->St_FLInstance);
    meshbuffer->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);
    this->meshList.push_back(meshbuffer);

    // Spawn Entities.
    entity* current = new entity();
    current->Init(Vector3(1.0f, 1.0f, 5.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
    current->text = &this->Teststring;
    current->type = entityType::eT_TextUI;
    current->meshptr = this->meshGetFast("saofontsheet");
    this->entitylists.push_back(current);
}

void Stateinit::OnRender()
{
    for (size_t i = 0; i < this->entitylists.size(); i++)
    {
        (*this->modelStack).LoadIdentity();
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
                                   this->entitylists[i]->position.z, // Used for Text SCaling. only applies to 2d UI 
                                   this->entitylists[i]->position.x, // Same as before
                                   this->entitylists[i]->position.y);// Same as before
        }
        (*this->modelStack).PopMatrix();
    }
}

void Stateinit::OnCam(int X, int Y, float XChange, float YChange)
{
    this->Teststring = std::to_string(X) + "|" + std::to_string(XChange) + "|" + std::to_string(Y) + "|" + std::to_string(YChange) + "|";
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
}
