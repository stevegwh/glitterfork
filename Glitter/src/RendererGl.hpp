//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include "Model.hpp"

#include <string>

#include <glad/glad.h>
#include <entt/entt.hpp>

namespace sage
{
struct Camera;
class RendererGl
{
    entt::registry* registry;
    Camera* cam;
    void cleanup();
    //std::vector<std::unique_ptr<Model>> renderables;
public:
    explicit RendererGl(Camera* _cam, entt::registry* _registry);
    ~RendererGl();
    void Render();
    //void AddRenderable(std::unique_ptr<Model> renderable);
};

} // sage
