//
// Created by Steve Wheeler on 16/04/2024.
//

#include "Application.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Transform.hpp"

#include <memory>
#include <iostream>
#include <random>

#include <SDL2/SDL.h>
#include <glad/glad.h>


namespace sage
{
bool applyImpulse;
// process all input: query SDL2 whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput()
{
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE])
        quit = true;

    while (SDL_PollEvent(&event))
    {
        cam.HandleEvent(&event);
        if (event.type == SDL_QUIT)
            quit = true;
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            int width, height;
            SDL_GetWindowSize(window, &width, &height);
            glViewport(0, 0, width, height);
        }

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_P]) 
        {
            std::cout << "Bombs away!" << std::endl;
            applyImpulse = true;
        }
    }
}

int Application::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    // Create window
    window = SDL_CreateWindow("LearnOpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_WarpMouseInWindow(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

    glEnable(GL_DEPTH_TEST);
    return 1;
}

void Application::update()
{
    clock.tick();
    fpsCounter.Update();
    //physics.StepSimulation();
    processInput();
//    physics.Update(clock.delta);
//    if (applyImpulse)
//    {
//        physics.ApplyImpulse({0,0,0}, {1,-10,1});
//    }


    //std::cout << fpsCounter.fps_current << std::endl;
    cam.Update(clock.delta);

    
}

void Application::draw()
{
    renderer.Render();
    SDL_GL_SwapWindow(window);
}

void Application::initPhysics()
{
    
}

void Application::initLevel()
{
    {
    auto floor = registry.create();
    registry.emplace<Transform>(floor, glm::vec3(0, -15, -10), glm::vec3(0, 35, 0), glm::vec3(40, 0.25, 40));
    
    registry.emplace<Model>(floor,
                            BINARY_PATH + "resources/cube_steve.obj",
                            new Shader(std::string(BINARY_PATH + "Shaders/shader.vert"),
                                       std::string(BINARY_PATH + "Shaders/shader.frag")));
    //physics.AddBoxObject(floor);
    }
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 zgen(rd()); // seed the generator
    std::mt19937 xgen(rd());
    std::uniform_int_distribution<> distr(-20, 20); // define the range
    
    auto* model = new Model(BINARY_PATH + "resources/cube_steve.obj",
                new Shader(std::string(BINARY_PATH + "Shaders/shader.vert"),
                           std::string(BINARY_PATH + "Shaders/shader.frag")));
    for (int i = 0; i < 30; ++i) 
    {
        auto entity = registry.create();
        registry.emplace<Transform>(entity, glm::vec3(distr(xgen), -14, distr(zgen)), glm::vec3(0, 35.0f, 0), glm::vec3(1.0f), 1.0f);
        Model* model2 = model;
        registry.emplace<Model>(entity, *model2);
        //physics.AddBoxObject(entity);
    }
    for (int i = 0; i < 10; ++i)
    {
        auto entity = registry.create();
        registry.emplace<Transform>(entity, glm::vec3(distr(xgen), 30, distr(zgen)), glm::vec3(0, 35.0f, 0), glm::vec3(1.0f), 1.0f);
        Model* model2 = model;
        registry.emplace<Model>(entity, *model2);
        //physics.AddBoxObject(entity);
    }
    // Currently, shader is not deleted and is leaking memory. Need to have a shader manager or something.
}

void Application::Run()
{
    initLevel();
    while (!quit)
    {
        update();
        draw();
    }
}

Application::Application() :
    cam(Camera({0, -2, 30}, {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, fov, zFar, zNear)),
    renderer(RendererGl(&cam, &registry))
//    physics(bPhysicsSystem(&registry))
{
    quit = false;
    initSDL();
}

Application::~Application()
{
    // SDL2: terminate, clearing all previously allocated SDL2 resources.
    // ------------------------------------------------------------------
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

} // sage   