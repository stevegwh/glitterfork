//
// Created by Steve Wheeler on 16/04/2024.
//

#include <glm/glm.hpp>

#pragma once

namespace sage
{

class Camera
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
public:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    float lastX = 400, lastY = 300;
    float yaw = -90, pitch = 0;
    float fov = 90.0f;
};

} // sage
