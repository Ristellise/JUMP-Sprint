#include "MouseHandler.h"


MouseHandler::MouseHandler(double sensitvity)
{
    this->sensitivity = sensitvity;
}


MouseHandler::~MouseHandler()
{
}

void MouseHandler::Update(GLFWwindow* window, double dt, bool reset)
{
    glfwGetCursorPos(window,&this->XChange, &this->YChange);
    double oldposX = this->XChange;
    double oldposY = this->YChange;
    this->XChange = (this->XChange - this->X) * dt * this->sensitivity;
    this->YChange = (this->YChange - this->Y) * dt * this->sensitivity;

    this->X = (int)oldposX;
    this->Y = (int)oldposY;
}

void MouseHandler::Center(GLFWwindow* window)
{
    glfwGetWindowSize(window, &X, &Y);
    glfwSetCursorPos(window, (double)X / 2, (double)Y / 2);
}