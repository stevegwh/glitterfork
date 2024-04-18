//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include "Camera.hpp"
#include <string>
#include <GLFW/glfw3.h>

namespace sage
{

class Application
{
    GLFWwindow* window;
    sage::Camera cam;
    bool firstMouse = true;
    int initGlfw();
    void processInput();
    void handleMouse();
    void handleScroll(double xoffset, double yoffset);
public:
    Application();
    ~Application();
    void Update();
    std::string BINARY_PATH;
};

} // sage
