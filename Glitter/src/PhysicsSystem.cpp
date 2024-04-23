//
// Created by Steve Wheeler on 23/04/2024.
//

#include "PhysicsSystem.hpp"
#include "Transform.hpp"

namespace sage
{

void PhysicsSystem::Update()
{
    if (!shouldUpdate) return;
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

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
        // TODO: Instead of printing, you want to update the entt transforms
        auto enttTrans = registry->get<Transform>(entityIndices[j]);
        registry->patch<Transform>(entityIndices[j], [n = trans](auto &t) { 
            t.position.x = n.getOrigin().getX(); 
            t.position.y = n.getOrigin().getY();
            t.position.z = n.getOrigin().getZ();
//            t.scale.x = 1; 
//            t.scale.y = 1; 
//            t.scale.z = 1;
            t.rotation.x = n.getRotation().getX(); 
            t.rotation.y = n.getRotation().getY(); 
            t.rotation.z = n.getRotation().getZ();
            //t.mass = t.mass;
        });
        //printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    }
}

void PhysicsSystem::AddBoxObject(entt::entity entity)
{
    
    auto transform = registry->get<Transform>(entity);
    // TODO: Initialise a bullet object and add it in

    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(transform.scale.x), 
                                                             btScalar(transform.scale.y), 
                                                             btScalar(transform.scale.z)));
    
    entityIndices[collisionShapes.size()] = entity;
    collisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(transform.position.x, 
                                        transform.position.y, 
                                        transform.position.z));

    btScalar mass(transform.mass);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
    {
        groundShape->calculateLocalInertia(mass, localInertia);
    }
    
    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    auto* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    auto* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    
}

PhysicsSystem::PhysicsSystem(entt::registry* _registry) :
    registry(_registry),
    collisionConfiguration(std::make_unique<btDefaultCollisionConfiguration>()), 
    dispatcher(std::make_unique<btCollisionDispatcher>(collisionConfiguration.get())),
    overlappingPairCache(new btDbvtBroadphase()), 
    solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
    dynamicsWorld(std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache, solver.get(), collisionConfiguration.get()))
{
    //int i;
    dynamicsWorld->setGravity(btVector3(0, -1, 0));

    ///create a few basic rigid bodies

//    //the ground is a cube of size 100 at position y = -56.
//    //the sphere will hit it at y = -6, with center at -5
//    {
//        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
//
//        collisionShapes.push_back(groundShape);
//
//        btTransform groundTransform;
//        groundTransform.setIdentity();
//        groundTransform.setOrigin(btVector3(0, -56, 0));
//
//        btScalar mass(0.);
//
//        //rigidbody is dynamic if and only if mass is non zero, otherwise static
//        bool isDynamic = (mass != 0.f);
//
//        btVector3 localInertia(0, 0, 0);
//        if (isDynamic)
//            groundShape->calculateLocalInertia(mass, localInertia);
//
//        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
//        auto* myMotionState = new btDefaultMotionState(groundTransform);
//        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
//        auto* body = new btRigidBody(rbInfo);
//
//        //add the body to the dynamics world
//        dynamicsWorld->addRigidBody(body);
//    }
//
//    {
//        //create a dynamic rigidbody
//        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
//        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
//        collisionShapes.push_back(colShape);
//
//        /// Create Dynamic Objects
//        btTransform startTransform;
//        startTransform.setIdentity();
//
//        btScalar mass(1.f);
//
//        //rigidbody is dynamic if and only if mass is non zero, otherwise static
//        bool isDynamic = (mass != 0.f);
//
//        btVector3 localInertia(0, 0, 0);
//        if (isDynamic)
//            colShape->calculateLocalInertia(mass, localInertia);
//
//        startTransform.setOrigin(btVector3(2, 10, 0));
//
//        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//        auto* myMotionState = new btDefaultMotionState(startTransform);
//        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//        auto* body = new btRigidBody(rbInfo);
//
//        dynamicsWorld->addRigidBody(body);
//    }
}

PhysicsSystem::~PhysicsSystem()
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
