//
// Created by Steve Wheeler on 16/04/2024.
//

#include "RendererGl.hpp"

#include "config.hpp"
#include "Camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

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
    auto view = registry->view<const Model, const Transform>();
    view.each([this](const auto& r, auto& t)
              {
                  glm::mat4 modelMat = t.GetMatrix();
                  glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, cam->zNear, cam->zFar);
                  glUniformMatrix4fv(glGetUniformLocation(r.shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
                  glUniformMatrix4fv(glGetUniformLocation(r.shader->getID(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));
                  glUniformMatrix4fv(glGetUniformLocation(r.shader->getID(), "view"), 1, GL_FALSE, glm::value_ptr(cam->view));
                  r.Draw();
              });
}

//void RendererGl::Render()
//{
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    for (const auto& model: renderables) 
//    {
//        glm::mat4 modelMat = model->transform->GetMatrix();
//        glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, cam->zNear, cam->zFar);
//        glUniformMatrix4fv(glGetUniformLocation(model->shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
//        glUniformMatrix4fv(glGetUniformLocation(model->shader->getID(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));
//        glUniformMatrix4fv(glGetUniformLocation(model->shader->getID(), "view"), 1, GL_FALSE, glm::value_ptr(cam->view));
//        model->Draw();
//    }
//}

void RendererGl::cleanup()
{
    
}

RendererGl::RendererGl(Camera* _cam, entt::registry* _registry) :
    registry(_registry), cam(_cam)
{
}

RendererGl::~RendererGl()
{
    cleanup();
}

//void RendererGl::AddRenderable(std::unique_ptr<Model> renderable)
//{
//    renderables.push_back(std::move(renderable));
//}
} // sage