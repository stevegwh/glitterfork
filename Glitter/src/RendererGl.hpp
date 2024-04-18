//
// Created by Steve Wheeler on 16/04/2024.
//

#pragma once

#include <string>
#include "Camera.hpp"
#include <glad/glad.h>

namespace sage
{

class RendererGl
{
    sage::Camera cam;
    bool firstMouse = true;
    void cleanup();
public:
    RendererGl();
    ~RendererGl();
    void Render();
    unsigned int LoadTexture(const std::string& path, GLenum textureNumber, GLenum format);
};

} // sage
