//
// Created by Steve Wheeler on 16/04/2024.
//

#include "RendererGl.hpp"

#include "config.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace sage
{

void RendererGl::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (const auto& renderable: renderables) 
    {
        auto* model = renderable->model.get();
        glm::mat4 modelMat = renderable->GetMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, cam->zNear, cam->zFar);
        glUniformMatrix4fv(glGetUniformLocation(model->shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
        glUniformMatrix4fv(glGetUniformLocation(model->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(model->shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(cam->view));
        model->Draw();
    }
}

void RendererGl::cleanup()
{
    
}

RendererGl::RendererGl(sage::Camera* _cam) :
    cam(_cam)
{
}

RendererGl::~RendererGl()
{
    cleanup();
}

void RendererGl::AddRenderable(std::unique_ptr<Renderable> renderable)
{
    renderables.push_back(std::move(renderable));
}
} // sage