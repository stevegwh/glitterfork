//
// Created by Steve Wheeler on 24/04/2024.
//

#pragma once

#include <entt/entt.hpp>

namespace sage
{

class jPhysicsSystem
{
    entt::registry* registry;
    
public:
    explicit jPhysicsSystem(entt::registry* _registry);

};

} // sage
