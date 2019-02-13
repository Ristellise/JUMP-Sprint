#pragma once
#include "Application.h"
class MouseHandler
{
private:
    double sensitivity = 10.0f;
public:
    double XChange = 0.0;
    double YChange = 0.0;
    int X = 0;
    int Y = 0;
    MouseHandler(double sensitvity = 10.0f);
    ~MouseHandler();
    void Update(GLFWwindow * window, double dt, bool reset = false);
    void Center(GLFWwindow * window);
};

