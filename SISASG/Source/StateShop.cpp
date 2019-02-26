#include "StateShop.h"
#include "LoadTGA.h"

StateShop::StateShop()
{
    this->StateName = "Shop";
}

void StateShop::OnEnter()
{
    this->STData->shopSelect = 0;

    Mesh* meshbuffer;

    // Ship 1
    meshbuffer = MeshBuilder::GenerateOBJ("ship1", "OBJ//Ship1.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//Ship1.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Ship 1
    entity* ship1 = new entity();
    ship1->Init(Vector3(0, 4, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
    ship1->type = entityType::eT_Object;
    ship1->name = "ship1";
    ship1->size = Vector3(5.f, 5.f, 5.f);
    ship1->meshptr = this->meshGetFast("ship1");
    this->entitylists->insert_or_assign("ship1", ship1);

    // Ship 2
    meshbuffer = MeshBuilder::GenerateOBJ("ship2", "OBJ//Ship2.obj")[0];
    meshbuffer->textureID = LoadTGA("TGA//Ship2.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Ship 2
    entity* ship2 = new entity();
    ship2->Init(Vector3(-40, 4, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
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
     ship3->Init(Vector3(-80, 4, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
     ship3->type = entityType::eT_Object;
     ship3->name = "ship3";
     ship3->size = Vector3(5.f, 5.f, 5.f);
     ship3->meshptr = this->meshGetFast("ship3");
     this->entitylists->insert_or_assign("ship3", ship3);

    // Side
    meshbuffer = MeshBuilder::GenerateQuad("sides", Color(0, 0, 0), 1);
    meshbuffer->textureID = LoadTGA("TGA//hangarsides.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    this->STData->SoundSrcs.insert_or_assign("testTrack", new SoundContainer(&this->STData->VERYLOUD, "Audio/xmtest.xm", SourceType::ST_OPENMPT));
    this->STData->SoundSrcs["testTrack"]->play(true);
}

void StateShop::OnExit()
{
}

void StateShop::OnUpdate(double dt)
{
    if (this->winMan->IsKeyPressed('R'))
    {
        // money update
        this->STData->fileSaver.setValue("money", std::to_string(this->STData->moneyData));

        // ship unlocks
        this->STData->fileSaver.setValue("ship2", std::to_string(this->STData->ship2unlock));
        this->STData->fileSaver.setValue("ship3", std::to_string(this->STData->ship3unlock));

        // save
        this->STData->fileSaver.saveFile("config.ini");

        this->readyExitlocal = true;
        this->spawnState = "Menus";
    }

    static int rotateDir = 1;
    static const float ROTATE_SPEED = 15.f;
    rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

    // For movement
    // static const float LSPEED = 10.0f;
    static const float CSHIFT = 40.f;

    if ((this->winMan->IsKeyPressed('A') ||
        (this->winMan->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && Dir == 1)) &&
        (state_cam->position.x < 0) && Delay == 0)
    {
        this->STData->shopSelect -= 1;
        Delay += 10;
        Shift = CSHIFT / Delay;
        shiftmovement = true;
    }

    if ((this->winMan->IsKeyPressed('D') ||
        (this->winMan->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && Dir == -1)) &&
        (state_cam->position.x > (-CSHIFT * (NumberOfShips - 1)) && Delay == 0))
    {
        this->STData->shopSelect += 1;
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
}

void StateShop::OnRender()
{
    switch (this->STData->shopSelect)
    {
    case 0:
        this->RenderTextScreen(this->STData->font, "Imperator", Color(255, 255, 255), 4.f, 9.f, 9.5f);
        this->RenderTextScreen(this->STData->font, "FREE", Color(255, 255, 255), 4.f, 17.5f, 9.5f);

        this->RenderTextScreen(this->STData->font, "70 Vel", Color(255, 255, 255), 4.f, 1.f, 6.5f);
        this->RenderTextScreen(this->STData->font, "70 Acl", Color(255, 255, 255), 4.f, 1.f, 5.5f);
        
        this->RenderTextScreen
        (
            this->STData->font, 
            "Standard issue spacecraft designed for mass production", 
            Color(255, 255, 255), 
            3.f, 6.5f, 0.f
        );

        lockUnlock = 1;
        break;
    case 1:
        this->RenderTextScreen(this->STData->font, "De Carrier", Color(255, 255, 255), 4.f, 9.f, 9.5f);
        this->RenderTextScreen(this->STData->font, "$1000", Color(255, 255, 255), 4.f, 17.5f, 9.5f);

        this->RenderTextScreen(this->STData->font, "90 Vel", Color(255, 255, 255), 4.f, 1.f, 6.5f);
        this->RenderTextScreen(this->STData->font, "60 Acl", Color(255, 255, 255), 4.f, 1.f, 5.5f);

        this->RenderTextScreen
        (
            this->STData->font,
            "Industry grade frigate primarily used to transport heavy minerals",
            Color(255, 255, 255),
            3.f, 6.f, 0.f
        );

        lockUnlock = this->STData->ship2unlock;
        break;
    case 2:
        this->RenderTextScreen(this->STData->font, "SUPER MEGA ULTRA HYPER BEAM", Color(255, 255, 255), 4.f, 7.f, 9.5f);
        this->RenderTextScreen(this->STData->font, "$3000", Color(255, 255, 255), 4.f, 17.5f, 9.5f);

        this->RenderTextScreen(this->STData->font, "120 Vel", Color(255, 255, 255), 4.f, 1.f, 6.5f);
        this->RenderTextScreen(this->STData->font, "500 Acl", Color(255, 255, 255), 4.f, 1.f, 5.5f);

        this->RenderTextScreen
        (
            this->STData->font,
            "Slap this thruster on they said. It would be awesome they said.",
            Color(255, 255, 255),
            3.f, 6.f, 0.f
        );

        lockUnlock = this->STData->ship3unlock;
        break;
    }

    switch (lockUnlock)
    {
    case 0:
        this->RenderTextScreen(this->STData->font, "Locked", Color(255.f, 0.f, 0.f), 4.f, 1.f, 8.5f);
        break;
    case 1:
        this->RenderTextScreen(this->STData->font, "Unlocked", Color(0.f, 255.f, 255.f), 4.f, 1.f, 8.5f);
        break;
    }

    this->RenderTextScreen(this->STData->font, "Money: $" + std::to_string(this->STData->moneyData), Color(255, 255, 255), 4.f, 1.f, 9.5f);
    RenderShips();
    RenderUI();
}

void StateShop::OnCam(int X, int Y, float XChange, float YChange)
{
}

void StateShop::RenderShips()
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
            (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
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

void StateShop::RenderUI()
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
    else if (this->winMan->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && this->STData->bounceTime <= 0.0)
    {
        this->STData->bounceTime = 0.3;

        // Code for checking price
        switch (this->STData->shopSelect)
        {
        case 0:
            costOfShip = 0;
            break;
        case 1:
            costOfShip = 1000;
            break;
        case 2:
            costOfShip = 3000;
            break;
        }

        // if the player has enough money
        if (this->STData->moneyData >= costOfShip)
        {
            // find which spacecraft is being looked at
            switch (this->STData->shopSelect)
            {
            case 0:
                break;
            case 1:
                // if it is not already unlocked
                if (this->STData->ship2unlock == false)
                {
                    // Code for editing ship unlock
                    this->STData->ship2unlock = true;
                    // Code for editing money
                    this->STData->moneyData -= costOfShip;
                }
                break;
            case 2:
                // if it is not already unlocked
                if (this->STData->ship3unlock == false)
                {
                    // Code for editing ship unlock
                    this->STData->ship3unlock = true;
                    // Code for editing money
                    this->STData->moneyData -= costOfShip;
                }
                break;
            }
        }
    }

    glEnable(GL_DEPTH_TEST);
    (*this->modelStack).PopMatrix();
}

StateShop::~StateShop()
{
}