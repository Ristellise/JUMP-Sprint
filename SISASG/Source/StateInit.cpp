#include "StateInit.h"
#include <iostream>
void Stateinit::OnRender()
{

    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(0, 0, 0);
    this->RenderText(meshList[GEO_TEXT], "AAAAAAAAAAAAAA", Color(0, 0, 0));
    (*this->modelStack).PopMatrix();
}

void Stateinit::OnEnter()
{
    std::cout << "Entering: " << this->StateName << std::endl;
    // Use OnEnter to push entities to the list of entities
    //this.
}
void Stateinit::OnExit()
{
    std::cout << "Shutdown: " << this->StateName << std::endl;
}

Stateinit::Stateinit()
{
    this->StateName = "init";
}

void Stateinit::OnUpdate()
{
}
