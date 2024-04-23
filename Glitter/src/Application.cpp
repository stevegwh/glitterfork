//
// Created by Steve Wheeler on 16/04/2024.
//

#include "Application.hpp"


#include <glad/glad.h>
#include <memory>

#include "Shader.hpp"
#include "Model.hpp"
#include "Renderable.hpp"
#include "Transform.hpp"

#include <iostream>
#include <SDL2/SDL.h>


namespace sage
{
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
}

void Application::update()
{
    clock.tick();
    fpsCounter.Update();
    std::cout << fpsCounter.fps_current << std::endl;
    cam.Update(clock.delta);
    processInput();
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
    auto shader = std::make_unique<Shader>(std::string(BINARY_PATH + "Shaders/shader.vert"), std::string(BINARY_PATH + "Shaders/shader.frag"));
    auto model = std::make_unique<Model>(BINARY_PATH + "resources/spyrolevel.obj", std::move(shader));
    
    // MEMORY LEAK
    auto* transform = new Transform(glm::vec3(0), glm::vec3(0), glm::vec3(0.1f));
    // -----
    
    auto renderable = std::make_unique<Renderable>(std::move(model), transform);
    renderer.AddRenderable(std::move(renderable));
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
    cam(Camera({0, 0, 5}, {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, fov, zFar, zNear)),
    renderer(RendererGl(&cam))
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