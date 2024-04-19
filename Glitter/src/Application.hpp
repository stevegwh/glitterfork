//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include "Camera.hpp"
#include "utils.hpp"
#include <string>
#include <SDL2/SDL.h>

namespace sage
{

class Application
{
    static constexpr double SCREEN_WIDTH = 1280;
    static constexpr double SCREEN_HEIGHT = 720;
    static constexpr float zFar = 1000;
    static constexpr float zNear = 0.1;
    static constexpr float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
    static constexpr float fov = 60;
    static constexpr unsigned long screenSize = SCREEN_WIDTH * SCREEN_HEIGHT;
    
    Clock clock;
    FPSCounter fpsCounter;
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