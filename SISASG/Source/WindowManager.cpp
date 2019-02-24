#include "WindowManager.h"
#include <iostream>


WindowManager::WindowManager()
{
}

void WindowManager::setWindow(GLFWwindow* window)
{
    this->handle = window;
}

bool WindowManager::IsKeyPressed(int key)
{
    return glfwGetKey(this->handle, key);
}

bool WindowManager::IsButtonPressed(int button)
{
    std::cout << glfwGetMouseButton(this->handle, button) << std::endl;
    return glfwGetMouseButton(this->handle, button);
}

WindowManager::~WindowManager()
{
}
