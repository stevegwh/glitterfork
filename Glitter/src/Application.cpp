//
// Created by Steve Wheeler on 16/04/2024.
//

#include "Application.hpp"

#include "slib.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
//include "RendererGl.hpp"
#include "Model.hpp"

#include <iostream>
#include <SDL2/SDL.h>


namespace sage
{



// process all input: query SDL2 whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE])
        quit = true;
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

void Application::Update()
{
    // SDL2: initialize and configure
    // sage::RendererGl rendererGl;
    sage::Shader shader(std::string(BINARY_PATH + "Shaders/shader.vert"), std::string(BINARY_PATH + "Shaders/shader.frag"));
    sage::Model model(BINARY_PATH + "resources/spyrolevel.obj", shader);
    model.scale = { 1.0f, 1.0f, 1.0f };

    // render loop
    // -----------
    while (!quit)
    {
        clock.tick();
        fpsCounter.Update();
        std::cout << fpsCounter.fps_current << std::endl;
        cam.Update(clock.delta);
        // input
        // -----
        processInput();
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 modelMat = model.GetMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, zNear, zFar);
        glUniformMatrix4fv(glGetUniformLocation(model.shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(model.shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(model.shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(cam.view));
        model.Draw();

        // SDL2: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        SDL_GL_SwapWindow(window);
        SDL_Event event;
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
}

Application::Application() :
    cam(sage::Camera({0, 0, 5}, {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, fov, zFar, zNear))
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