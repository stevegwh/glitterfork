//
// Created by AICDG on 2017/8/9.
//

#pragma once

#include <btBulletCollisionCommon.h>

#include <iostream>

namespace sage
{
class bPhysicsCustomMotionState: public btDefaultMotionState
{
public:
    explicit bPhysicsCustomMotionState(const btTransform &transform)
        : btDefaultMotionState(transform)
    {}

    void getWorldTransform(btTransform& centerOfMassWorldTrans) const override
    {
        centerOfMassWorldTrans = m_graphicsWorldTrans * m_centerOfMassOffset.inverse();
    }
};
}