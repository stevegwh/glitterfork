//
// Created by Steve Wheeler on 23/04/2024.
//

#pragma once

#include "btBulletDynamicsCommon.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <memory>

namespace sage
{
struct PhysicsObject
{
    entt::entity entityId;
    btRigidBody* rigidBody;
};
class bPhysicsSystem
{
    entt::registry* registry;
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache;
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
public:
    bool shouldUpdate = false;
    explicit bPhysicsSystem(entt::registry* _registry);
    ~bPhysicsSystem();
    void Update();
    void AddBoxObject(entt::entity entity);
    void ApplyImpulse(const glm::vec3& origin, const glm::vec3& impulse);
};
} //sage
