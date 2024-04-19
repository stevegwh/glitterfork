//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include <string>
#include "Camera.hpp"
#include "Renderable.hpp"
#include <glad/glad.h>

namespace sage
{

class RendererGl
{
    sage::Camera* cam;
    bool firstMouse = true;
    void cleanup();
    std::vector<std::unique_ptr<Renderable>> renderables;
public:
    RendererGl(sage::Camera* _cam);
    ~RendererGl();
    void Render();
    void AddRenderable(std::unique_ptr<Renderable> renderable);
};

} // sage
