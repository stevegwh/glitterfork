//
// Created by Steve Wheeler on 23/04/2024.
//

#pragma once

#include <glm/glm.hpp>

namespace sage
{
struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    [[nodiscard]] glm::mat4 GetMatrix() const;
    Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale);
};
} //sage
