//
// Created by Steve Wheeler on 16/04/2024.
//

#include "Application.hpp"

#include "config.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "RendererGl.hpp"
#include "Model.hpp"

#include <iostream>
#include <SDL2/SDL.h>


namespace sage
{

float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void Application::handleMouse()
{
    int xpos, ypos;
    SDL_GetMouseState(&xpos, &ypos);
    if (firstMouse)
    {
        cam.lastX = xpos;
        cam.lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - cam.lastX;
    float yoffset = cam.lastY - ypos;
    cam.lastX = xpos;
    cam.lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam.yaw   += xoffset;
    cam.pitch += yoffset;

    if(cam.pitch > 89.0f)
        cam.pitch = 89.0f;
    if(cam.pitch < -89.0f)
        cam.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    direction.y = sin(glm::radians(cam.pitch));
    direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.cameraFront = glm::normalize(direction);
}

void Application::handleScroll(double xoffset, double yoffset)
{
    cam.fov += (float)yoffset;
    if (cam.fov < 1.0f)
        cam.fov = 1.0f;
    if (cam.fov > 90.0f)
        cam.fov = 90.0f;
}

// process all input: query SDL2 whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE])
        quit = true;

    float cameraSpeed = 2.5f * deltaTime;
    if (state[SDL_SCANCODE_W])
        cam.cameraPos += cameraSpeed * cam.cameraFront;
    if (state[SDL_SCANCODE_S])
        cam.cameraPos -= cameraSpeed * cam.cameraFront;
    if (state[SDL_SCANCODE_A])
        cam.cameraPos -= glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
    if (state[SDL_SCANCODE_D])
        cam.cameraPos += glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;

    handleMouse();
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
                              SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

    glEnable(GL_DEPTH_TEST);
}

Application::Application()
{
    quit = false;
    initSDL();
}

void Application::Update()
{
    // SDL2: initialize and configure
    // sage::RendererGl rendererGl;
    sage::Shader shader(std::string(BINARY_PATH + "Shaders/shader.vert"), std::string(BINARY_PATH + "Shaders/shader.frag"));
    sage::Model model(BINARY_PATH + "resources/cube_steve.obj", shader);

    // render loop
    // -----------
    while (!quit)
    {
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput();
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraFront, cam.cameraUp);
        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), model.position);
        modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(cam.fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000.0f);
        glUniformMatrix4fv(glGetUniformLocation(model.shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(model.shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(model.shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model.Draw();

        // SDL2: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        SDL_GL_SwapWindow(window);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
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

Application::~Application()
{
    // SDL2: terminate, clearing all previously allocated SDL2 resources.
    // ------------------------------------------------------------------
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

} // sage   