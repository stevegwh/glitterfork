//
// Created by Steve Wheeler on 23/04/2024.
//


#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace sage
{

Transform::Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) :
position(_position), rotation(_rotation), scale(_scale), mass(0)
{}

Transform::Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, float _mass) :
    position(_position), rotation(_rotation), scale(_scale), mass(_mass)
{}

glm::mat4 Transform::GetMatrix() const
{
    auto out = glm::mat4(1.0f);
    out = glm::translate(out, position);
    out = glm::rotate(out, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    out = glm::rotate(out, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    out = glm::rotate(out, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    out = glm::scale(out, scale);
    return out;
}

} //sage