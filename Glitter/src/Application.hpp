//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "RendererGl.hpp"
#include "Camera.hpp"
#include "utils.hpp"

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

    sage::Camera cam;
    RendererGl renderer;
    
    Clock clock;
    FPSCounter fpsCounter;
    SDL_Window* window;
    SDL_GLContext context;

    bool quit;
    int initSDL();
    void processInput();
    void update();
    void draw();
public:
    Application();
    ~Application();
    void Run();
    std::string BINARY_PATH;
};

} // sage