#include <iostream>
#include "phy_scene.hpp"
#include "manifold.hpp"

glm::vec2 kGravity{0, -9.81f};

void PhyScene::Step(float dt) {
    for (RigidBody* rbody : rbodies) {
        IntegrateForces(rbody, dt);
    }

    std::vector<Manifold> manifolds;
    for(size_t i = 0; i < rbodies.size(); i++) {
        for(size_t j = i+1; j < rbodies.size(); j++) {
            manifolds.emplace_back(rbodies[i], rbodies[j]);
            manifolds.back().ApplyImpulse();
        }
    }

    for (RigidBody* rbody : rbodies) {
        IntegrateVelocities(rbody, dt);
    }

    // for (Manifold& m : manifolds) {
    //     m.PositionalCorrection();
    // }
}

void PhyScene::AddRigidBody(RigidBody* body) {
    rbodies.push_back(body);
}

void PhyScene::IntegrateForces(RigidBody* body, float dt) {
    if (body->inverse_mass == 0.0f)
        return;

    body->velocity += (body->acceleration + kGravity) * dt;
    body->angular_velocity += body->angular_acceleration * dt;
}
void PhyScene::IntegrateVelocities(RigidBody* body, float dt) {
    if (body->inverse_mass == 0.0f)
        return;

    body->position += body->velocity * dt;
    body->orientation += body->angular_velocity * dt;
}
