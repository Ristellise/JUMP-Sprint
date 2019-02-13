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
    
    this->XChange = this->sensitivity * dt * float(X / 2 - this->XChange);
    this->YChange = this->sensitivity * dt * float(Y / 2 - this->YChange);
}

void MouseHandler::Center(GLFWwindow* window)
{
    glfwGetWindowSize(window, &X, &Y);
    glfwSetCursorPos(window, (double)X / 2, (double)Y / 2);
}