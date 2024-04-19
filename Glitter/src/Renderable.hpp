//
// Created by Steve Wheeler on 19/04/2024.
//

#pragma once

#include <utility>
#include <glm/glm.hpp>
#include "Model.hpp"

namespace sage
{
struct Renderable
{
    std::unique_ptr<Model> model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Renderable(std::unique_ptr<Model> _model,
               glm::vec3 _position,
               glm::vec3 _rotation, // euler angles
               glm::vec3 _scale) :
        model(std::move(_model)),
        position(_position),
        rotation(_rotation),
        scale(_scale)
    {};
    [[nodiscard]] glm::mat4 GetMatrix() const;

};
}