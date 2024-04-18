//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include "Camera.hpp"
#include <string>
#include <SDL2/SDL.h>

namespace sage
{

class Application
{
    SDL_Window* window;
    SDL_GLContext context;
    sage::Camera cam;
    bool firstMouse = true;
    bool quit;
    int initSDL();
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