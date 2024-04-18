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
    static constexpr double SCREEN_WIDTH = 1280;
    static constexpr double SCREEN_HEIGHT = 720;
    static constexpr float zFar = 1000;
    static constexpr float zNear = 0.1;
    static constexpr float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
    static constexpr float fov = 120;
    static constexpr unsigned long screenSize = SCREEN_WIDTH * SCREEN_HEIGHT;
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
