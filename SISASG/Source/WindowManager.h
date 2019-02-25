#pragma once
#include "Application.h"

// Why this? Because blame Ms Tan.
// Keypresses are NOW actually only active on the active window...
class WindowManager
{
private:
    bool exit = false;
public:
    GLFWwindow* handle;
    bool getExit() { return this->exit; }
    bool setExit() { this->exit = true; }
    bool IsKeyPressed(int key);
    bool IsButtonPressed(int button);
    WindowManager();
    void setWindow(GLFWwindow * window);
    ~WindowManager();
};

