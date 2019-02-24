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
    selectingShips = true;
    this->STData->shipSelect = 0;
    this->STData->planetSelect = 0;

    Mesh* meshbuffer;

    // this->state_cam->Init(Vector3(0, 4, -40), Vector3(0, 4, 1), Vector3(0, 1, 0));

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
    ship2->Init(Vector3(-40, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
    ship2->type = entityType::eT_Object;
    ship2->name = "ship2";
    ship2->size = Vector3(5.f, 5.f, 5.f);
    ship2->meshptr = this->meshGetFast("ship2");
    this->entitylists->insert_or_assign("ship2", ship2);

    //Ship 3
    meshbuffer = MeshBuilder::GenerateOBJ("ship3", "OBJ//Ship3.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//Ship3.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Ship 3
    entity* ship3 = new entity();
    ship3->Init(Vector3(-80, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
    ship3->type = entityType::eT_Object;
    ship3->name = "ship3";
    ship3->size = Vector3(5.f, 5.f, 5.f);
    ship3->meshptr = this->meshGetFast("ship3");
    this->entitylists->insert_or_assign("ship3", ship3);

    // Venus
    meshbuffer = MeshBuilder::GenerateOBJ("venus", "OBJ//Planet sphere.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//venus texture.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Earth
    meshbuffer = MeshBuilder::GenerateOBJ("earth", "OBJ//Planet sphere.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//earth texture.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Mars
    meshbuffer = MeshBuilder::GenerateOBJ("mars", "OBJ//Planet sphere.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//mars texture.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Jupiter
    meshbuffer = MeshBuilder::GenerateOBJ("jupiter", "OBJ//Planet sphere.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//jupiter texture.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

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
    delete this->entitylists->find("ship3")->second;
    this->entitylists->erase("ship3");

    delete this->entitylists->find("ship2")->second;
    this->entitylists->erase("ship2");

    delete this->entitylists->find("ship1")->second;
    this->entitylists->erase("ship1");
}

void StateHangar::OnUpdate(double dt)
{
    if (Application::IsKeyPressed('R'))
    {
        this->readyExitlocal = true;
        this->spawnState = "Menus";
    }

    static int rotateDir = 1;
    static const float ROTATE_SPEED = 10.f;
    rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

    if (selectingShips == true)
    {
        // For movement
        // static const float LSPEED = 10.0f;
        static const float CSHIFT = 40.f;

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
    }
    else
    {
        // For movement
        // static const float LSPEED = 10.0f;
        static const float CSHIFT = 30.f;

        if ((Application::IsKeyPressed('A') ||
            ((Application::IsKeyPressed(MK_LBUTTON)) && Dir == 1)) &&
            (state_cam->position.x < -120) && Delay == 0)
        {
            this->STData->planetSelect -= 1;
            Delay += 10;
            Shift = CSHIFT / Delay;
            shiftmovement = true;
        }

        if ((Application::IsKeyPressed('D') ||
            ((Application::IsKeyPressed(MK_LBUTTON)) && Dir == -1)) &&
            (state_cam->position.x > (-CSHIFT * (NumberOfPlanets - 1)) - 120 && Delay == 0))
        {
            this->STData->planetSelect += 1;
            Delay += 10;
            Shift = -CSHIFT / Delay;
            shiftmovement = true;
        }
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
    if (selectingShips == true)
    {
        switch (this->STData->shipSelect)
        {
        case 0:
            lockUnlock = 1;
            break;
        case 1:
            lockUnlock = this->STData->ship2unlock;
            break;
        case 2:
            lockUnlock = this->STData->ship3unlock;
            break;
        }

        switch (lockUnlock)
        {
        case 0:
            this->RenderTextScreen(this->STData->font, "Locked", Color(255.f, 0.f, 0.f), 4.f, 1.f, 9.5f);
            break;
        case 1:
            this->RenderTextScreen(this->STData->font, "Unlocked", Color(0.f, 255.f, 255.f), 4.f, 1.f, 9.5f);
            break;
        }
    }
    else
    {
        switch (this->STData->planetSelect)
        {
        case 0:
            this->RenderTextScreen(this->STData->font, "Venus", Color(255, 255, 255), 4.f, 9.2f, 9.5f);
            break;
        case 1:
            this->RenderTextScreen(this->STData->font, "Earth", Color(255, 255, 255), 4.f, 9.2f, 9.5f);
            break;
        case 2:
            this->RenderTextScreen(this->STData->font, "Mars", Color(255, 255, 255), 4.f, 9.2f, 9.5f);
            break;
        case 3:
            this->RenderTextScreen(this->STData->font, "Jupiter", Color(255, 255, 255), 4.f, 9.2f, 9.5f);
            break;
        }
    }
    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(-120, 4, 0);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(8.f, 8.f, 8.f);
    RenderMesh(this->meshGetFast("venus"), true);
    (*this->modelStack).PopMatrix();

    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(-150, 4, 0);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(8.4f, 8.4f, 8.4f);
    RenderMesh(this->meshGetFast("earth"), true);
    (*this->modelStack).PopMatrix();

    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(-180, 4, 0);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(6.0f, 6.0f, 6.0f);
    RenderMesh(this->meshGetFast("mars"), true);
    (*this->modelStack).PopMatrix();

    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(-210, 4, 0);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(12.f, 12.f, 12.f);
    RenderMesh(this->meshGetFast("jupiter"), true);
    (*this->modelStack).PopMatrix();

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
                buff->position.z,    // Used for Text SCaling. only applies to 2d UI 
                buff->position.x,    // Same as before
                buff->position.y);    // Same as before
        }
        (*this->modelStack).PopMatrix();
    }
}

// For left/right arrow buttons
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
        
        if (selectingShips == true)
        {
            switch (this->STData->shipSelect)
            {
            case 0:
                lockUnlock = 1;
                break;
            case 1:
                lockUnlock = this->STData->ship2unlock;
                break;
            case 2:
                lockUnlock = this->STData->ship3unlock;
                break;
            }

            switch (lockUnlock)
            {
            case 0:
                break;
            case 1:
                Delay += 10;
                Shift = (-120.f - state_cam->position.x) / Delay;
                shiftmovement = true;
                selectingShips = false;
                break;
            }
        }
        else if (selectingShips == false)
        {
            this->readyExitlocal = true;
            this->spawnState = "Game";
        }

        /*
        this->readyExitlocal = true;
        this->spawnState = "Game";
        */
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
        stars[i].x = 100.f + ((10000.f * 0.9f) * sin(phi) * cos(theta));
        stars[i].y = 100.f + ((10000.f * 0.9f) * sin(phi) * sin(theta));
        stars[i].z = 100.f + ((10000.f * 0.9f) * cos(phi));
    }
}

StateHangar::~StateHangar()
{
}
