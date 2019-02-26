#include "StateInit.h"
#include "MeshBuilder.h"
#include <iostream>
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "collision.h"
#include "Bullet.h"

void Stateinit::OnEnter()
{
    // initialize file readwrite
    this->STData->fileSaver.loadFile("config.ini");

    // Load ini file into memory
    std::string moneyDat = this->STData->fileSaver.getValueString("money");

    this->STData->moneyData = std::stoull(moneyDat);
    this->STData->ship2unlock = this->STData->fileSaver.getValueBool("ship2");
    this->STData->ship3unlock = this->STData->fileSaver.getValueBool("ship3");

    this->spawnState = "Menus";

    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
    this->projectionStack->LoadMatrix(projection);

    // Load meshes first.
    Mesh* meshbuffer;
    meshbuffer = MeshBuilder::GenerateText("saofontsheet", *this->St_FLInstance);
    meshbuffer->textureID = LoadTGA("Font//fnt_0.tga", GL_LINEAR, GL_REPEAT);
    this->meshList->push_back(meshbuffer);

    meshbuffer = MeshBuilder::GenerateSphere("debugballs", Color(1, 1, 1), 10, 10, 0.25f);
    this->meshList->push_back(meshbuffer);

	meshbuffer = MeshBuilder::GenerateSphere("bullet", Color(1, 1, 1), 18, 36, 0.5);
	this->meshList->push_back(meshbuffer);

	// Camera
	this->state_cam->Init(Vector3(0, 4, -40), Vector3(0, 4, 1), Vector3(0, 1, 0));
    // Camera
    this->state_cam->Init(Vector3(0, 4, -40), Vector3(0, 4, 1), Vector3(0, 1, 0));

    // Spawn Entities.
    entity* current = new entity();

    // Debugging string
    // Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
    current->Init(Vector3(1.f, 24.f, 2.f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
    current->text = &dtimestring;
    current->type = entityType::eT_TextUI;
    current->meshptr = this->meshGetFast("saofontsheet");
    this->entitylists->insert_or_assign("saofontsheet",current);

    // Sound
    INIFile SoundDef;
    SoundDef.loadFile(this->STData->fileSaver.getValueString("DefsFolder") + "/sound.ini");
    std::map<std::string, std::string>::iterator soundit;
    std::map<std::string, std::string> mapbuff;
    mapbuff = SoundDef.getValuesAll();
    for (soundit = mapbuff.begin(); soundit != mapbuff.end(); soundit++)
    {
        
        std::vector<std::string> Sndbuff = SoundDef.getValueMulti(soundit->first, ',');
        bool stream = false;
        if (Sndbuff[1] == "1")
        {
            stream = true;
        }
        this->STData->SoundSrcs.insert_or_assign(soundit->first, new SoundContainer(&this->STData->VERYLOUD, Sndbuff[0], srcTypeFromExtension(Sndbuff[0], stream)));
        
        if (Sndbuff[2] == "1")
        {
            this->STData->SoundSrcs[soundit->first]->play(true, true);
            this->STData->SoundSrcs[soundit->first]->loopPos(std::stof(Sndbuff[2]));
            this->STData->SoundSrcs[soundit->first]->enableLooping();
        }
        else
        {
            this->STData->SoundSrcs[soundit->first]->play(true, true);
        }
    }
	// Debugging string
	// Init: Only the first Vector3 matters. Format: (translateX, translateY, scale) This is for TextUI
	current->Init(Vector3(1.f, 24.f, 2.f), Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	current->text = &dtimestring;
	current->type = entityType::eT_TextUI;
	current->meshptr = this->meshGetFast("saofontsheet");
	this->entitylists->insert_or_assign("saofontsheet", current);
}

void Stateinit::OnRender()
{

}

void Stateinit::OnCam(int X, int Y, float XChange, float YChange)
{
}

void Stateinit::OnExit()
{
    this->STData->VERYLOUD.deinit();
    std::cout << "Shutdown: " << this->StateName << std::endl;
}

Stateinit::Stateinit()
{
    this->StateName = "init";
}

void Stateinit::OnUpdate(double dt)
{
    if (this->STData->bounceTime >= 0.0)
    {
        this->STData->bounceTime -= dt;
    }
    this->STData->timeBegin += dt;
}
