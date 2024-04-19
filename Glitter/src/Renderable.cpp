//
// Created by Steve Wheeler on 19/04/2024.
//

#include "Renderable.hpp"

namespace sage
{
glm::mat4 Renderable::GetMatrix() const
{
    glm::mat4 out = glm::mat4(1.0f);
    out = glm::translate(out, position);
    out = glm::rotate(out, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    out = glm::rotate(out, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    out = glm::rotate(out, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    out = glm::scale(out, scale);

    return out;
}
}
