#pragma once

#include "rigidbody.hpp"

class PhyScene {
public:
    void Step(float dt);
    void AddRigidBody(RigidBody* body);

private:
    std::vector<RigidBody*> rbodies;

    void IntegrateForces(RigidBody* body, float dt);
    void IntegrateVelocities(RigidBody* body, float dt);
};
