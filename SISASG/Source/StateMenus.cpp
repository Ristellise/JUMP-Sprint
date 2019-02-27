#include "StateMenus.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"

StateMenus::StateMenus()
{
    this->StateName = "Menus";
}

void StateMenus::OnEnter()
{
    // Camera reset
    this->state_cam->Reset();

    Mesh* meshbuffer;

    // sun
    meshbuffer = MeshBuilder::GenerateOBJ("sun", "OBJ//Planet sphere.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//sun texture.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // asteroid
    meshbuffer = MeshBuilder::GenerateOBJ("asteroid", "OBJ//asteroid1.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//asteroid1 texture.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    rotateAngle = 0;
    movement_asteroid1_z = 0;

    this->STData->SoundSrcs["title"]->unpause(1.0f);
}

void StateMenus::OnExit()
{
}

void StateMenus::OnUpdate(double dt)
{
    this->dtimestring = "\nCURSOR: " + std::to_string(this->mouse->X) +
        " | " + std::to_string(this->mouse->Y) +
        " | SPEED: " + std::to_string(this->mouse->XChange) +
        " | " + std::to_string(this->mouse->YChange);

    static int rotateDir_asteroid = 1;
    static int rotateDir = -1;
    static const float ROTATE_SPEED = 8.f;
    rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

    movement_asteroid1_z += (float)(rotateDir_asteroid * ROTATE_SPEED * dt);

    if (movement_asteroid1_z * rotateDir_asteroid > 40)
    {
        rotateDir_asteroid = -rotateDir_asteroid;
    }
    if (movement_asteroid1_z < -40 && rotateDir_asteroid < -40)
    {
        rotateDir_asteroid = -rotateDir_asteroid;
    }
}

void StateMenus::OnRender()
{
    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(-300, 0, 600);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(300.f, 300.f, 300.f);
    RenderMesh(this->meshGetFast("sun"), true);
    //(*this->modelStack).PopMatrix();

    //(*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(2, 0, 0);
    (*this->modelStack).Rotate(rotateAngle * 5, 1, 0, 1);
    (*this->modelStack).Scale(0.005f, 0.005f, 0.005f);
    RenderMesh(this->meshGetFast("asteroid"), true);
    (*this->modelStack).PopMatrix();
      
    this->RenderTextScreen(this->STData->font, "SISASG", Color(255, 255, 255), 6.f, 1.3f, 4.5f);
    this->RenderTextScreen(this->STData->font, this->dtimestring, Color(255, 255, 255), 2.f, 4.f, 21.f);

    // Ship Hangar button
    if ((this->mouse->X > 100) && (this->mouse->X < 330) && (this->mouse->Y >450) && (this->mouse->Y < 520))
    {
        this->RenderTextScreen(this->STData->font, " > Start Game", Color(255, 255, 255), 6.f, 1.f, -1.f);
        
        if (this->winMan->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && this->STData->bounceTime <= 0.0)
        {
            this->STData->bounceTime = 0.3;
            this->readyExitlocal = true;
            this->spawnState = "Hangar";
        }
    }
    else
    {
        this->RenderTextScreen(this->STData->font, "   Start Game", Color(255, 255, 255), 6.f, 1.f, -1.f);
    }

    // Ship Foundry button
    if ((this->mouse->X > 100) && (this->mouse->X < 210) && (this->mouse->Y > 520) && (this->mouse->Y < 590))
    {
        this->RenderTextScreen(this->STData->font, " > Shop", Color(255, 255, 255), 6.f, 1.f, -2.f);

        if (this->winMan->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && this->STData->bounceTime <= 0.0)
        {
            this->STData->bounceTime = 0.3;
            this->readyExitlocal = true;
            this->spawnState = "Shop";
            this->STData->SoundSrcs["title"]->pause(5.0f);
        }
    }
    else
    {
        this->RenderTextScreen(this->STData->font, "   Shop", Color(255, 255, 255), 6.f, 1.f, -2.f);
    }
}

void StateMenus::OnCam(int X, int Y, float XChange, float YChange)
{
}

StateMenus::~StateMenus()
{
}
