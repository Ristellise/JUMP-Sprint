#include "StateInit.h"
#include <iostream>


void Stateinit::OnEnter()
{
    
    //this->entitylists.push_back();
    std::cout << "Entering: " << this->StateName<< " Albion Prelude."<< std::endl;
    // Use OnEnter to push entities to the list of entities
    //this.
    this->modelStack = new MS();
    this->viewStack = new MS();
    this->projectionStack = new MS();
}

void Stateinit::OnRender()
{
    (*this->modelStack).LoadIdentity();
    (*this->modelStack).PushMatrix();
    (*this->modelStack).Translate(0, 0, 0);
    //this->RenderText(meshList[GEO_TEXT], "AAAAAAAAAAAAAA", Color(0, 0, 0));
    (*this->modelStack).PopMatrix();
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
