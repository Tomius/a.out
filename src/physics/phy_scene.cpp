#include <iostream>
#include <glbinding/gl33core/gl.h>
#include "phy_scene.hpp"
#include "manifold.hpp"

namespace gl33 = gl33core;

glm::vec2 kGravity{0, -9.81f};

void PhyScene::Step(float dt) {
    // Prevent exterme lag from causing an "infinite loop"
    if (dt > 0.256f) {
        dt = 0.256f;
    }

    while (dt > kIterationInterval) {
        DoStep(kIterationInterval);
        dt -= kIterationInterval;
    }
    DoStep(dt);
}

void PhyScene::DoStep(float dt) {
    manifolds.clear();

    for (RigidBody* rbody : rbodies) {
        IntegrateForces(rbody, dt);
    }

    for (RigidBody* rbody : rbodies) {
        rbody->UpdateBounderCache();
    }

    for (size_t i = 0; i < rbodies.size(); i++) {
        for (size_t j = i+1; j < rbodies.size(); j++) {
            manifolds.emplace_back(rbodies[i], rbodies[j]);
            manifolds.back().ApplyImpulse();
        }
    }

    for (RigidBody* rbody : rbodies) {
        IntegrateVelocities(rbody, dt);
    }
}

void PhyScene::DebugDraw(const Video::Camera& camera) const {
    gl33::glPointSize(4.0f);
    for (const Manifold& manifold : manifolds) {
        manifold.DebugDraw(camera);
    }
    gl33::glPointSize(1.0f);
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
