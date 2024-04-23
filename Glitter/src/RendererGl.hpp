//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include <string>
#include "Renderable.hpp"
#include <glad/glad.h>

namespace sage
{
struct Camera;
class RendererGl
{
    sage::Camera* cam;
    void cleanup();
    std::vector<std::unique_ptr<Renderable>> renderables;
public:
    explicit RendererGl(Camera* _cam);
    ~RendererGl();
    void Render();
    void AddRenderable(std::unique_ptr<Renderable> renderable);
};

} // sage
