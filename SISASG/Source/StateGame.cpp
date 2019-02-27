#include "StateGame.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"
#include "genericEntity.h"
#include "Bullet.h"
#include "asteroidEnt.h"

StateGame::StateGame()
{
    this->StateName = "Game";
}


StateGame::~StateGame()
{
}

void StateGame::OnEnter()
{
    this->STData->SoundSrcs["title"]->unpause();
    hoopGenerate(); // Generates the locations of the hoops based on the selected planet
    this->STData->asteroidSmashed = 0;
    // Reset stats
    this->STData->moneyEarned = 0;
    this->STData->pointsPrev = 0;
    this->STData->timePrev = 0;

    // Camera reset
    this->state_cam->Reset();
    
    // Resets
    elapsedTime = 60;
    points = 0;

    // Matrix method
    cubeMatrix.SetToIdentity();
    cubeMultR.SetToIdentity();
    cubeMult1.SetToIdentity();
    cubeMult2.SetToIdentity();
    cubeMult3.SetToIdentity();

    Mesh* meshbuffer;

    meshbuffer = meshGetFast("spaceship");
    meshisnull = false;
    if (meshbuffer == nullptr)
    {
        meshisnull = true;
    }
    // Ship loading
    switch (this->STData->shipSelect)
    {
    case 0:
        // Ship1
        meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship1.obj")[0];
        meshbuffer->textureID = LoadTGA("TGA//Ship1.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 1:
        // Ship2
        meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship2.obj")[0];
        meshbuffer->textureID = LoadTGA("TGA//Ship2.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 2:
        // Ship3
        meshbuffer = MeshBuilder::GenerateOBJ("spaceship", "OBJ//Ship3.obj")[0];
        meshbuffer->textureID = LoadTGA("TGA//Ship3.tga", GL_LINEAR, GL_CLAMP);
        break;
    }    
    if (meshisnull)
    {
        this->meshList->push_back(meshbuffer);
    }

    // Spaceship
    spaceship* spaceship1 = new spaceship();
    spaceship1->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
    spaceship1->setWindow(this->winMan);
    spaceship1->type = entityType::eT_Ship;
    spaceship1->name = "spaceship";
    spaceship1->physics = true;
    spaceship1->Boxsize = BBoxDimensions(2.5f, 2.5f, 2.5f);
    spaceship1->meshptr = meshbuffer;

    switch (this->STData->shipSelect)
    {
    case 0:
        spaceship1->topSpeed = 70.0f;
        spaceship1->fwdaccl = 70.f;
        spaceship1->bwdaccl = -40.f;
        spaceship1->drift = 7.0f;
        Exhausts.push_back(Exhaust);
        Exhausts.push_back(Exhaust);
        break;
    case 1:
        spaceship1->topSpeed = 90.0f;
        spaceship1->fwdaccl = 60.f;
        spaceship1->bwdaccl = -30.f;
        spaceship1->drift = 10.f;
        Exhausts.push_back(Exhaust);
        Exhausts.push_back(Exhaust);
        Exhausts.push_back(Exhaust);
        break;
    case 2:
        spaceship1->topSpeed = 120.0f;
        spaceship1->fwdaccl = 500.f;
        spaceship1->bwdaccl = -20.f;
        spaceship1->drift = 5.0f;
        Exhausts.push_back(Exhaust);
        break;
    }
    spaceship1->Reset();
    this->entitylists->insert_or_assign("spaceship", spaceship1);
    
	// Planets
    meshbuffer = meshGetFast("planet");
    meshisnull = false;
    if (meshbuffer == nullptr)
    {
        meshisnull = true;
        meshbuffer = MeshBuilder::GenerateOBJ("planet", "OBJ//Planet sphere.obj")[0];
    }
    switch (this->STData->planetSelect)
    {
    case 0:
        // Venus
        meshbuffer->textureID = LoadTGA("TGA//venus texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 1:
        // Earth
        meshbuffer->textureID = LoadTGA("TGA//earth texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 2:
        // Mars
        meshbuffer->textureID = LoadTGA("TGA//mars texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    case 3:
        // Jupiter
        meshbuffer->textureID = LoadTGA("TGA//jupiter texture.tga", GL_LINEAR, GL_CLAMP);
        break;
    }
    if (meshisnull)
    {
        this->meshList->push_back(meshbuffer);
    }

	// Sun
	meshbuffer = MeshBuilder::GenerateOBJ("sun", "OBJ//Planet sphere.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//sun texture.tga", GL_LINEAR, GL_CLAMP);
	this->meshList->push_back(meshbuffer);

    // Test Env
    // meshbuffer = MeshBuilder::GenerateOBJ("testenv", "OBJ//TestEnv.obj")[0];
    // meshbuffer->textureID = LoadTGA("TGA//TestEnv.tga", GL_LINEAR, GL_CLAMP);
    // this->meshList->push_back(meshbuffer);

    // Audio src
    // this->audiosrc.Load("Audio/testtrack.flac");

    /*
    // Test Env
    entity* testEnv = new entity();
    testEnv->Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
    testEnv->type = entityType::eT_Environment;
    testEnv->name = "testenv";
    // testEnv->physics = true;
    // testEnv->Boxsize = BBoxDimensions(0.f, 0.f, 0.f);
    testEnv->meshptr = this->meshGetFast("testenv");
    this->entitylists->insert_or_assign("testenv",testEnv);
    */

    // Hoops
    meshbuffer = MeshBuilder::GenerateTorus("hoop", Color(0.f, 255.f, 255.f), 36, 36, 15, 1);
    this->meshList->push_back(meshbuffer);

    // Particle
    meshbuffer = MeshBuilder::GenerateQuad("particle", Color(0.f, 255.f, 255.f),1);
    //meshbuffer = MeshBuilder::GenerateCube("particle", Color(0.f, 255.f, 255.f),1);
    meshbuffer->textureID = LoadTGA("TGA//testparticle.tga", GL_LINEAR, GL_CLAMP);
    this->meshList->push_back(meshbuffer);

    // Axes
    // meshbuffer = MeshBuilder::GenerateAxes("axes", 200, 200, 200);
    // this->meshList->push_back(meshbuffer);

	// Bullet
	meshbuffer = MeshBuilder::GenerateOBJ("bullet", "OBJ//Bullet.obj")[0];
	meshbuffer->textureID = LoadTGA("TGA//Bullet.tga", GL_LINEAR, GL_CLAMP);
	
	this->meshList->push_back(meshbuffer);

    bullet = new Bullet();
    bullet->Init(Vector3(spaceship1->position.x,spaceship1->position.y,spaceship1->position.z), Vector3(spaceship1->target.x,spaceship1->target.y,spaceship1->target.z), Vector3(0, 1, 0));
    bullet->type = entityType::eT_Bullet;
    bullet->Boxsize = BBoxDimensions(0.5f, 0.5f, 0.5f);
    bullet->physics = true;
    bullet->name = "bullet";
    bullet->meshptr = this->meshGetFast("bullet");

    // Collision tester
    /*
    entity* current = new genericEntity();
    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(0, 0, 1), Vector3(0, 1, 0));
    current->type = entityType::eT_Object;
    current->meshptr = this->meshGetFast("spaceship");
    current->physics = true;
    current->Boxsize = BBoxDimensions(0.5f, 0.5f, 0.5f);
    this->entitylists->insert_or_assign("testcube",current);
    */

    Hooplah hl;
    unsigned int cnter = 0;
    for (size_t i = 0; i < this->hoopPos.size(); i++)
    {
        hl = this->hoopPos[i];

		// random between 0 and 2 asteroids per hoop
        int rand = mt19937Rand(0, 2);

        for (size_t i = 0; i < rand; i++)
        {
            
            asteroidEnt* AstEntity = new asteroidEnt();
            //AstEntity->InitSound(this->STData->SoundSrcs["asteroidhit"]);
            AstEntity->Boxsize;
            AstEntity->Init(Vector3(hl.offset_x + mt19937Rand(-50.0f, 50.0f),
                                    hl.offset_y + mt19937Rand(-50.0f, 50.0f),
                                    hl.offset_z + mt19937Rand(-50.0f, 50.0f)), Vector3(0, 0, 1), Vector3(0, 1, 0));
            AstEntity->size = Vector3(mt19937Rand(2.f, 3.0f), mt19937Rand(2.f, 3.0f), mt19937Rand(2.f, 3.0f));
            AstEntity->type = entityType::eT_Object;
            AstEntity->meshptr = this->meshGetFast("asteroid");
            AstEntity->physics = true;
            AstEntity->pitchTotal = mt19937Rand(.0f, 360.0f);
            AstEntity->rollTotal = mt19937Rand(.0f, 360.0f);
            AstEntity->yawTotal = mt19937Rand(.0f, 360.0f);
            AstEntity->InitSound(this->STData->SoundSrcs["asteroidhit"], &this->STData->timeBegin);
            AstEntity->Boxsize = BBoxDimensions(AstEntity->size.x * 3.0f, AstEntity->size.y  * 3.0f, AstEntity->size.z  * 3.0f);
            this->entitylists->insert_or_assign("asteroid"+std::to_string(cnter), AstEntity);
            cnter++;
        }
    }
    

    Stars();
    std::cout << this->entitylists->size() << std::endl;
    std::cout << this->meshList->size() << std::endl;
}

void StateGame::OnExit()
{
    delete this->entitylists->find("spaceship")->second;
    this->entitylists->erase("spaceship");
    while (hoopPos.size()) // Clears the hoops
    {
        hoopPos.pop_back();
    }
    while (Exhausts.size()) // Clears particles
    {
        Exhausts.pop_back();
    }
    while (stars.size())
    {
        stars.pop_back();
    }
    std::map<std::string, entity*>::iterator it = this->entitylists->begin();
    while (it != this->entitylists->end())
    {
        if (it->first.find("asteroid") != -1 || it->first.find("bullet") != -1)
        {
            std::map<std::string, entity*>::iterator toErase = it;
            ++it;
            delete toErase->second;
            this->entitylists->erase(toErase);
        } 
        else {
            ++it;
        }
    }
}

void StateGame::OnUpdate(double dt)
{
    elapsedTime -= dt;

    static int rotateDir = 1;
    static const float ROTATE_SPEED = 10.f;
    rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

    entity* spaceship1 = this->entityGetFast("spaceship");

    this->dgamestring = "POINTS OBTAINED: ";
    this->dgamestring += std::to_string(points);
    this->dgamestring += "\nTIME REMAINING: ";
    this->dgamestring += std::to_string(elapsedTime);

    this->dtimestring = "\n\nFPS: ";
    this->dtimestring += std::to_string(1.0f / dt);
    this->dtimestring += "\nCAM X: ";
    this->dtimestring += std::to_string(this->state_cam->position.x);
    this->dtimestring += "\nCAM Y: ";
    this->dtimestring += std::to_string(this->state_cam->position.y);
    this->dtimestring += "\nCAM Z: ";
    this->dtimestring += std::to_string(this->state_cam->position.z);

    this->dtimestring += "\nVEL: ";
    this->dtimestring += std::to_string(spaceship1->velocity);
    this->dtimestring += "\nACL: ";
    this->dtimestring += std::to_string(spaceship1->accl);
    this->dtimestring += "\nPIT: ";
    this->dtimestring += std::to_string(spaceship1->pitchTotal);
    this->dtimestring += "\nYAW: ";
    this->dtimestring += std::to_string(spaceship1->yawTotal);
    this->dtimestring += "\nROL: ";
    this->dtimestring += std::to_string(spaceship1->rollTotal);

    if ((points >= totalHoops) || (elapsedTime <= 0.0))
    {
        if (elapsedTime < 0.0)
        {
            elapsedTime = 0.0;
        }

        this->STData->moneyEarned += (unsigned long long)(points * elapsedTime);

        this->STData->pointsPrev = points;
        this->STData->timePrev = elapsedTime;
        this->STData->moneyData += this->STData->moneyEarned;
        this->readyExitlocal = true;
        this->spawnState = "Stat";
    }

    hoopChecker();
    Exhaust.GenerateParticles(dt);
    Exhaust.ParticleUpdate(dt);
    Exhaust.setplocation(*spaceship1, spaceship1->position.x, spaceship1->position.y,spaceship1->position.z);
    
    //Bullet returning to ship after 0.5s of timeAlive
    if ((this->winMan->IsKeyPressed(GLFW_KEY_SPACE)))
    {
        Bullet* b = new Bullet;
        *b = *this->bullet;
        b->Init(spaceship1->position,spaceship1->target, spaceship1->up);
        b->velocity += spaceship1->velocity;
        this->entitylists->insert_or_assign("bullet" + std::to_string(bcount), b);
        bcount++;
        std::cout << "New bullet " << std::to_string(bcount) << std::endl;
    }

    hoopChecker();
    
    switch (STData->shipSelect)
    {
    case 0:
        Exhausts[0].setplocation(*spaceship1, -2, -2, -11);
        Exhausts[1].setplocation(*spaceship1, +2, -2, -11);
        break;
    case 1:
        Exhausts[0].setplocation(*spaceship1, +0, -1.5, -7);
        Exhausts[0].setRotateangle(0);
        Exhausts[0].setScale(3, 1, 1);
        Exhausts[1].setplocation(*spaceship1, +4, -3, -8);
        Exhausts[2].setplocation(*spaceship1, -4, -3, -8);
        break;
    case 2:
        Exhausts[0].setplocation(*spaceship1, +0, -2, -5);
        Exhausts[0].setRotateangle(1.0f);
        Exhausts[0].setScale(3, 3, 1);
        break;
    }

    for (unsigned int i = 0; Exhausts.size() > i; i++)
    {
        if (this->entityGetFast("spaceship")->velocity > 0)
        {
            Exhausts[i].GenerateParticles(dt);
        }
        Exhausts[i].ParticleUpdate(dt);
    }

    // Sound Updating
    this->STData->VERYLOUD.set3dListenerPosition(spaceship1->position.x, spaceship1->position.y, spaceship1->position.z);
    this->STData->VERYLOUD.set3dListenerUp(spaceship1->up.x, spaceship1->up.y, spaceship1->up.z);
    this->STData->VERYLOUD.set3dListenerAt(spaceship1->view.x, spaceship1->view.y, spaceship1->view.z);
    this->STData->VERYLOUD.update3dAudio();

    std::map<std::string, entity*>::iterator eit = this->entitylists->begin();

    while (eit != this->entitylists->end())
    {
        if (eit->second->doDestroy)
        {
            if (eit->second->type == entityType::eT_Object)
            {
                this->STData->asteroidSmashed++;
            }
            eit->second->OnDelete(&eit->second);
            
            std::map<std::string, entity*>::iterator toErase = eit;
            ++eit;
            this->entitylists->erase(toErase);
        }
        else
        {
            eit->second->Update(dt);
            ++eit;
        }
    }

    this->state_cam->Update(dt, *spaceship1);

    this->collideInstance->updatingEnts = 0;

    this->collideInstance->doCollisions(*this->entitylists, dt);

    if (this->winMan->IsKeyPressed('R'))
    {
        this->readyExitlocal = true;
        this->spawnState = "Menus";
    }

    

}

void StateGame::hoopChecker()
{
    ///////* start of planet and hoop stuff *///////

    entity* spaceship = this->entityGetFast("spaceship");

    // generates the hoop checkers
    for (unsigned int i = 0; i < hoopPos.size(); i++) // for loop follows array
    {
        // passes values into hoops for coords
        if ((hoop.hoopsCheckXY((int)hoopPos[i].offset_x, (int)hoopPos[i].offset_y, (int)hoopPos[i].offset_z, (int)spaceship->position.x, (int)spaceship->position.y, (int)spaceship->position.z, (int)rad, (int)hoopPos[i].rotation)) && hoopPos[i].passed == false)
        {
            points++;
            hoopPos[i].passed = true;
        }
    }

    ///////* end of planet and hoop stuff *///////
}

void StateGame::hoopRender()
{
    for (int i = 0; i < totalHoops; i++)
    {
        if (hoopPos[i].passed != true)
        {
            (*this->modelStack).PushMatrix(); // render the hoops
            (*this->modelStack).Translate(hoopPos[i].offset_x, hoopPos[i].offset_y, hoopPos[i].offset_z); // sets the coords of each hoop (coord stored in an array for each hoop)
            (*this->modelStack).Rotate(hoopPos[i].rotation, 1, 0, 0);
            RenderMesh(this->meshGetFast("hoop"), true);
            (*this->modelStack).PopMatrix();
        }
    }
}

void StateGame::hoopGenerate()
{

    // venus
    float the_addition = 10.f, the_subtraction = 0.f;

    switch (this->STData->planetSelect)
    {
    case(0): // venus
        z = 400.f;
        totalHoops = 5;
        for (int i = 0; i < totalHoops; i++)
        {
            hoopPos.push_back(ok);
            if (i > 2 && i < totalHoops)
            {
                the_subtraction -= 20;
                hoopPos[i].offset_x = x + the_subtraction * 2;
                hoopPos[i].offset_y = y + the_subtraction;
            }
            else
            {
                hoopPos[i].offset_x = x + the_addition * 2;
                hoopPos[i].offset_y = y + the_addition;
                the_subtraction += 20;
            }

            hoopPos[i].offset_z = z + the_addition * 15;

            the_addition += 30.f; // increases addition value so it keeps going

            if (i > 0)
            {
                if (hoopPos[i - 1].rotation == 90)
                {
                    hoopPos[i].rotation = 0;
                }
                else
                {
                    hoopPos[i].rotation += 45 + hoopPos[i - 1].rotation; // for rotation of hoops
                }
            }
        }
        break;
    case(1): // earth
        x = 0.f;
        z = 300.f;
        totalHoops = 6;
        for (int i = 0; i < totalHoops; i++)
        {
            hoopPos.push_back(ok);
            if (i > 2 && i < totalHoops)
            {
                the_subtraction -= 30.f;
                //hoopPos[i].offset_y = y + the_subtraction;
                hoopPos[i].offset_x = x + the_addition * 2;
                
            }
            else
            {
               // hoopPos[i].offset_y = y + the_addition * 3;
                hoopPos[i].offset_x = x + the_subtraction * 2;
                the_subtraction += 20.f;
            }

            hoopPos[i].offset_z = z + the_addition * 15;

            the_addition += 30.f; // increases addition value so it keeps going
           
            if (i > 0)
            {
                if (hoopPos[i - 1].rotation == 90)
                {
                    hoopPos[i].rotation = 0;
                }
                else
                {
                    hoopPos[i].rotation += 45 + hoopPos[i - 1].rotation; // for rotation of hoops
                }
            }
        }
        break;
    case(2): // mars
        x = 100.f;
        z = 500.f;
        totalHoops = 7;
        for (int i = 0; i < totalHoops; i++)
        {
            hoopPos.push_back(ok);
            if (i > 2 && i < totalHoops)
            {
                the_subtraction -= 30.f;
                hoopPos[i].offset_x = x - the_subtraction * 1.5f;
                hoopPos[i].offset_y = y - the_subtraction;
            }
            else if (i == totalHoops)
            {
                hoopPos[i].offset_x = x - the_addition;
                hoopPos[i].offset_y = y + the_addition;
            }
            else
            {
                hoopPos[i].offset_x = x - the_addition * 2;
                hoopPos[i].offset_y = y + the_addition;
                the_subtraction += 50.f;
            }

            hoopPos[i].offset_z = z + the_addition * 10;
            the_addition += 40.f; // increases addition value so it keeps going
            if (i > 0)
            {
                if (hoopPos[i - 1].rotation == 90)
                {
                    hoopPos[i].rotation = 0;
                }
                else
                {
                    hoopPos[i].rotation += 45 + hoopPos[i - 1].rotation; // for rotation of hoops
                }
            }
        }
        break;
    case(3): // jupiter
        x = 200.f;
        z = 400.f;
        totalHoops = 8;
        for (int i = 0; i < totalHoops; i++)
        {
            hoopPos.push_back(ok);
            if (i > 2 && i < 6)
            {
                the_subtraction += 50.f;
                hoopPos[i].offset_x = x - the_subtraction * 3.f;
                hoopPos[i].offset_y = y + the_subtraction;
            }
            else
            {
                hoopPos[i].offset_x = x - the_addition * 2.f;
                hoopPos[i].offset_y = y + the_addition;
                the_addition += 60.f;
            }
            hoopPos[i].offset_z = z + the_addition * 5;

            if (i > 0)
            {
                if (hoopPos[i - 1].rotation == 90)
                {
                    hoopPos[i].rotation = 0;
                }
                else
                {
                    hoopPos[i].rotation += 45 + hoopPos[i - 1].rotation; // for rotation of hoops
                }
            }
        }
        break;
    default:
        break;
    }

}

void StateGame::OnRender()
{
	// Sun
	(*this->modelStack).PushMatrix();
	(*this->modelStack).Translate(0.f, 0.f, -9500.0f);
	(*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
	(*this->modelStack).Scale(600.f, 600.f, 600.f);
	RenderMesh(this->meshGetFast("sun"), true);
	(*this->modelStack).PopMatrix();

    // Stars
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

    entity *spaceship = this->entityGetFast("spaceship");

    for (unsigned int j = 0; Exhausts.size() > j; j++)
    {
        for (unsigned int i = 0; Exhausts[j].particles.size() > i; i++)
        {
            (*this->modelStack).PushMatrix();
                
            (*this->modelStack).Translate(Exhausts[j].particles[i].Position.x, Exhausts[j].particles[i].Position.y, Exhausts[j].particles[i].Position.z);
            (*this->modelStack).Rotate(Exhausts[j].pRotateAngle * 10 * rotateAngle, spaceship->view.x, spaceship->view.y, spaceship->view.z);
            (*this->modelStack).MultMatrix(cubeMult2);
            (*this->modelStack).Scale(Exhausts[j].scale_x, Exhausts[j].scale_y, Exhausts[j].scale_z);
                
            RenderMesh(this->meshGetFast("particle"), false);
            (*this->modelStack).PopMatrix();
        }
    }

    // Planet
    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(400, 0, 1000);
    (*this->modelStack).Rotate(rotateAngle, 0, 1, 0);
    (*this->modelStack).Scale(200.f, 200.f, 200.f);
    RenderMesh(this->meshGetFast("planet"), true);
    (*this->modelStack).PopMatrix();

    hoopRender();

    this->RenderTextScreen(this->STData->font, this->dgamestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 4.f, 1.f, 9.5f);
    this->RenderTextScreen(this->STData->font, this->dtimestring, Color(0 / 255.f, 0 / 255.f, 0 / 255.f), 2.f, 2.f, 20.f);

    std::map<std::string, entity*>::iterator it = this->entitylists->begin();

    while (it != this->entitylists->end())
    {
        {
            (*this->modelStack).PushMatrix();
            entity *buff = it->second;
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
                (*this->modelStack).Rotate(buff->rollTotal, 0, 0, 1);
                (*this->modelStack).Scale(buff->size.x, buff->size.y, buff->size.z);
                RenderMesh(buff->meshptr, true);

            }
            else if (buff->type == entityType::eT_TextUI)
            {
                this->RenderTextScreen(buff->meshptr, *buff->text, Color(0, 0, 0),
                    buff->position.z,    // Used for Text Scaling. only applies to 2d UI 
                    buff->position.x,    // Same as before
                    buff->position.y);    // Same as before
            }
            else if (buff->type == entityType::eT_Ship)
            {
                entity *spaceship = this->entityGetFast("spaceship");

                // Matrix method v2
                (*this->modelStack).PushMatrix();
                cubeMult1.SetToTranslation(spaceship->position.x, spaceship->position.y, spaceship->position.z);

                if (spaceship->lKey == true)
                {
                    cubeMultR.SetToRotation(spaceship->angle, spaceship->up.x, spaceship->up.y, spaceship->up.z);
                    cubeMult2 = cubeMultR * cubeMult2;
                }

                if (spaceship->rKey == true)
                {
                    cubeMultR.SetToRotation(-(spaceship->angle), spaceship->up.x, spaceship->up.y, spaceship->up.z);
                    cubeMult2 = cubeMultR * cubeMult2;
                }

                if (spaceship->uKey == true)
                {
                    cubeMultR.SetToRotation(-(spaceship->angle), spaceship->right.x, spaceship->right.y, spaceship->right.z);
                    cubeMult2 = cubeMultR * cubeMult2;
                }

                if (spaceship->dKey == true)
                {
                    cubeMultR.SetToRotation(spaceship->angle, spaceship->right.x, spaceship->right.y, spaceship->right.z);
                    cubeMult2 = cubeMultR * cubeMult2;
                }

                if (spaceship->qKey == true)
                {
                    cubeMultR.SetToRotation(-(spaceship->angle), spaceship->view.x, spaceship->view.y, spaceship->view.z);
                    cubeMult2 = cubeMultR * cubeMult2;
                }

                if (spaceship->eKey == true)
                {
                    cubeMultR.SetToRotation(spaceship->angle, spaceship->view.x, spaceship->view.y, spaceship->view.z);
                    cubeMult2 = cubeMultR * cubeMult2;
                }

                cubeMult3.SetToScale(5.0f, 5.0f, 5.0f);
                cubeMatrix = cubeMult1 * cubeMult2 * cubeMult3;

                (*this->modelStack).LoadMatrix(cubeMatrix);
                RenderMesh(spaceship->meshptr, true);
                (*this->modelStack).PopMatrix();
            }
            else if (buff->type == entityType::eT_Environment)
            {
                (*this->modelStack).PushMatrix();
                (*this->modelStack).Scale(10.0f, 10.0f, 10.0f);
                RenderMesh(buff->meshptr, true);
                (*this->modelStack).PopMatrix();

			(*this->modelStack).PushMatrix();
			(*this->modelStack).Translate(0, 0, 0);
			RenderMesh(this->meshGetFast("axes"), false);
			(*this->modelStack).PopMatrix();
		}
		else if (buff->type == entityType::eT_Bullet)
		{
			
			(*this->modelStack).PushMatrix();
			(*this->modelStack).Translate(bullet->position.x,bullet->position.y,bullet->position.z);
			(*this->modelStack).Rotate(10 * rotateAngle, spaceship->view.x, spaceship->view.y, spaceship->view.z);
			(*this->modelStack).MultMatrix(cubeMult2);
			RenderMesh(this->meshGetFast("bullet"), true);
			(*this->modelStack).PopMatrix();
			
		}
		(*this->modelStack).PopMatrix();

            Vector3 temp[] = {
                buff->HBox.backLeftDown,
                buff->HBox.backLeftUp,
                buff->HBox.backRightDown,
                buff->HBox.backRightUp,
                buff->HBox.frontLeftDown,
                buff->HBox.frontLeftUp,
                buff->HBox.frontRightDown,
                buff->HBox.frontRightUp,
            };

            for (size_t i = 0; i < 8; i++)
            {
                (*this->modelStack).PushMatrix();
                (*this->modelStack).Translate(temp[i].x, temp[i].y, temp[i].z);
                (*this->modelStack).Scale(0.1f, 0.1f, 0.1f);
                RenderMesh(this->meshGetFast("star"), false);
                (*this->modelStack).PopMatrix();
            }
            ++it;
        }
    }
}

void StateGame::OnCam(int X, int Y, float XChange, float YChange)
{
}

void StateGame::Stars()
{
    starsnumber = 200; // Generated number of stars
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
