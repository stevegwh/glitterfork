//
// Created by Steve Wheeler on 23/04/2024.
//

#include "bPhysicsSystem.hpp"
#include "Transform.hpp"
#include "bPhysicsCustomMotionState.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

namespace sage
{

void bPhysicsSystem::StepSimulation()
{
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);
}

void bPhysicsSystem::Update(float deltaTime)
{
    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {

            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }

        auto entity = static_cast<entt::entity>(reinterpret_cast<uintptr_t>(obj->getUserPointer()));
        auto euler = glm::vec3();
        trans.getRotation().getEulerZYX(euler.z, euler.y, euler.x);
        registry->patch<Transform>(entity, [n= trans, e = euler](auto &t) { 
            t.position.x = n.getOrigin().x(); 
            t.position.y = n.getOrigin().y();
            t.position.z = n.getOrigin().z();
//            t.scale.x = old.scale.x; 
//            t.scale.y = old.scale.y; 
//            t.scale.z = old.scale.z;
            t.rotation.x = glm::degrees(e.x);
            t.rotation.y = glm::degrees(e.y);
            t.rotation.z = glm::degrees(e.z);
            t.mass = 1.0f;
        });
        //printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    }
}

void bPhysicsSystem::ApplyImpulse(const glm::vec3& origin, const glm::vec3& impulse)
{
    
    for (auto& body: bodies) 
    {
        if (body->getInvMass() != 0.0f)
        {
            std::cout << "Bombs away!" << std::endl;
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);

            btVector3 relativePosition = trans.getOrigin() - btVector3(origin.x, origin.y, origin.z);

            // Convert the impulse from glm::vec3 to btVector3
            btVector3 btImpulse(impulse.x, impulse.y, impulse.z);
            body->applyImpulse(btImpulse, relativePosition);
            auto entity = static_cast<entt::entity>(reinterpret_cast<uintptr_t>(body->getUserPointer()));
            // Do something here.
        }
    }
}

void bPhysicsSystem::AddBoxObject(entt::entity entity)
{
    auto transform = registry->get<Transform>(entity);
    
    btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(transform.scale.x), 
                                                             btScalar(transform.scale.y), 
                                                             btScalar(transform.scale.z)));
    
    collisionShapes.push_back(colShape);

    btTransform physicsTransform;
    physicsTransform.setIdentity();
    physicsTransform.setOrigin(btVector3(transform.position.x, 
                                        transform.position.y, 
                                        transform.position.z));
    btQuaternion quat;
    quat.setEulerZYX(btScalar(glm::radians(transform.rotation.z)),
                     btScalar(glm::radians(transform.rotation.y)),
                     btScalar(glm::radians(transform.rotation.x)));
    physicsTransform.setRotation(quat);
    

    btScalar mass(transform.mass);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
    {
        colShape->calculateLocalInertia(mass, localInertia);
    }
    
    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
//    auto* myMotionState = new btDefaultMotionState(physicsTransform);
    auto* myMotionState = new bPhysicsCustomMotionState(physicsTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    auto* body = new btRigidBody(rbInfo);
    
    // store ECS entity ID of the associated transform/gameobject
    body->setUserPointer(reinterpret_cast<void*>(static_cast<uintptr_t>(entity)));
    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    bodies.push_back(body);
}

bPhysicsSystem::bPhysicsSystem(entt::registry* _registry) :
    registry(_registry),
    collisionConfiguration(std::make_unique<btDefaultCollisionConfiguration>()), 
    dispatcher(std::make_unique<btCollisionDispatcher>(collisionConfiguration.get())),
    overlappingPairCache(new btDbvtBroadphase()), 
    solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
    dynamicsWorld(std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache, solver.get(), collisionConfiguration.get()))
{
    dynamicsWorld->setGravity(btVector3(0, -1, 0));
}

bPhysicsSystem::~bPhysicsSystem()
{
    delete overlappingPairCache;

    //remove the rigidbodies from the dynamics world and delete them
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    //next line is optional: it will be cleared by the destructor when the array goes out of scope
    collisionShapes.clear();
}
} //sage
