//
// Created by Steve Wheeler on 19/04/2024.
//

#include "Renderable.hpp"

namespace sage
{


Renderable::Renderable(Model* _model, sage::Transform *_transform) :
    model(_model),
    transform(_transform)
{}
}
