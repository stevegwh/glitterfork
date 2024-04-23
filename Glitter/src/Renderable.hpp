//
// Created by Steve Wheeler on 19/04/2024.
//

#pragma once

#include <utility>
#include "Model.hpp"
#include "Transform.hpp"

namespace sage
{
struct Renderable
{
    Model* model;
    Transform* const transform;
    Renderable(Model* _model, Transform* _transform);
};
}