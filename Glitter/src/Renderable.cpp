//
// Created by Steve Wheeler on 19/04/2024.
//

#include "Renderable.hpp"

namespace sage
{


Renderable::Renderable(std::unique_ptr<Model> _model, sage::Transform *_transform) :
    model(std::move(_model)),
    transform(_transform)
{}
}
