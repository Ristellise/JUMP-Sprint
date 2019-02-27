#include "StateMagic.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
StateMagic::StateMagic()
{
    this->StateName = "secrets";
}


StateMagic::~StateMagic()
{
}

void StateMagic::OnEnter()
{
    std::cout << "Magisk Started." << std::endl;
    Mesh* buff;
    buff = MeshBuilder::GenerateQuad("sinon", Color(0, 0, 0), 1.0f);
    buff->textureID = LoadTGA("TGA/sinon.tga", GL_LINEAR, GL_REPEAT);
    this->meshList->push_back(buff);

    buff = MeshBuilder::GenerateQuad("blackbg", Color(0, 0, 0), 1.0f);
    this->meshList->push_back(buff);
}

void StateMagic::OnExit()
{
}

void StateMagic::OnUpdate(double dt)
{
    this->dtimestring = "\nCURSOR: " + std::to_string(this->mouse->X) +
        " | " + std::to_string(this->mouse->Y);
    bool seckey = this->winMan->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && this->winMan->IsKeyPressed(GLFW_KEY_LEFT_SHIFT);
    if (seckey && this->winMan->IsKeyPressed('O') && !replaced)
    {

        this->STData->SoundSrcs["title"]->DIE(true);
        this->STData->SoundSrcs["title"] = this->STData->SoundSrcs["overcursed"];
        this->STData->SoundSrcs["title"]->enableLooping();
        this->STData->SoundSrcs["title"]->play(true);
        std::cout << "Replaced Menu with an Overly Cursed music..." << std::endl;
        this->replaced = true;
    }
    if (seckey && this->winMan->IsKeyPressed('U') && !credits)
    {
        std::cout << "Displaying Credits..." << std::endl;
        credits = true;
        this->STData->SoundSrcs["title"]->pause(1.0f);
        this->STData->SoundSrcs["forgetmenot"]->setSeek(0.0f);
        this->STData->SoundSrcs["forgetmenot"]->unpause(1.0f);
    }
    if (this->winMan->IsKeyPressed(GLFW_KEY_BACKSPACE) && credits)
    {
        std::cout << "Hiding Credits..." << std::endl;
        this->STData->SoundSrcs["title"]->unpause(1.0f);
        
        this->STData->SoundSrcs["forgetmenot"]->pause(1.0f);
        credits = false;
    }
    else if (this->winMan->IsKeyPressed(GLFW_KEY_BACKSPACE) && seckey)
    {
        std::cout << "Hiding Magic Scene..." << std::endl;
        this->readyExitlocal = true;
        this->STData->secreton = false;
        this->STData->SoundSrcs["forgetmenot"]->pause(1.0f);
        this->STData->SoundSrcs["title"]->unpause(1.0f);
    }
}

void StateMagic::Set2D()
{
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10); // size of screen UI
    (*this->projectionStack).PushMatrix();
    (*this->projectionStack).LoadMatrix(ortho);
    (*this->viewStack).PushMatrix();
    (*this->viewStack).LoadIdentity(); // No need for camera ortho mode
    (*this->modelStack).PushMatrix();
    (*this->modelStack).LoadIdentity(); // Reset modelStack
}

void StateMagic::Wrap2D()
{
    (*this->projectionStack).PopMatrix();
    (*this->viewStack).PopMatrix();
    (*this->modelStack).PopMatrix();
}

void StateMagic::OnRender()
{
    if (credits)
    {
        int x = 0;
        int y = 0;
        this->winMan->getWindowSize(x, y);
        
        this->Set2D(); // Begins the UI code.
        
        
        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(70.f, 50.f, 0.0f);
            (*this->modelStack).Scale(10.f, 10.f, 0.0f);
            this->RenderMesh(this->meshGetFast("sinon"),false);
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(40.f, 27.f, 0.0f);
            (*this->modelStack).Scale(6.0f, 6.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "CREDITS", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(32.f, 27.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "==Shinon Productions==", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(38.f, 22.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "~Developers~", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
        (*this->modelStack).Translate(42.f, 17.f, 0.0f);
        (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
        this->RenderText(this->meshGetFast("saofontsheet"), "Shinon", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(39.f, 12.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "Imaxinacion", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(42.f, 7.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "Danish", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(38.f, 2.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "Bluewarriorz", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();

        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(36.f, -3.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "ChocolateSauce", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();
        
        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(20.f, -20.f, 0.0f);
            (*this->modelStack).Scale(5.0f, 5.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "Press \"Backspace\" To Exit This Credits Scene", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();
        this->Wrap2D(); // Wrap up 2D UI.


        
    }
    else
    {
        this->RenderTextScreen(this->STData->font, this->dtimestring, Color(255, 255, 255), 2.f, 4.f, 21.f);
        this->Set2D(); // Begins the UI code.
        (*this->modelStack).PushMatrix();
            (*this->modelStack).Translate(2.0f, -4.f, 0.0f);
            (*this->modelStack).Scale(2.0f, 2.0f, 0.0f);
            this->RenderText(this->meshGetFast("saofontsheet"), "Magisk Mode Actived.\nCtrl + Shift + Backspace to turn off Magisk Mode", Color(0, 0, 0));
        (*this->modelStack).PopMatrix();
        this->Wrap2D(); // Wrap up 2D UI.
    }
    
}

void StateMagic::OnCam(int X, int Y, float XChange, float YChange)
{
}
