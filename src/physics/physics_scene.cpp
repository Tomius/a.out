#include <iostream>
#include <glbinding/gl33core/gl.h>
#include "physics_scene.hpp"
#include "collision.hpp"
#include "manifold.hpp"
#include "r4k.hpp"

namespace gl33 = gl33core;

void PhysicsScene::Step(float dt) {
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

void PhysicsScene::DoStep(float dt) {
    manifolds.clear();

    std::vector<PhysicsState> states;

    // predict
    for (RigidBody* rbody : rbodies) {
        states.emplace_back (*rbody);
        R4KIntegrate(rbody, dt);
    }

    for (RigidBody* rbody : rbodies) {
        rbody->UpdateBounderCache();
    }

    for (size_t i = 0; i < rbodies.size(); i++) {
        for (size_t j = i+1; j < rbodies.size(); j++) {
            Manifold manifold{rbodies[i], rbodies[j]};
            if (!manifold.IsEmpty ()) {
                manifolds.push_back(manifold);
            }
        }
    }

    // Undo the predicition
    for (size_t i = 0; i < rbodies.size(); ++i) {
        *static_cast<PhysicsState*>(rbodies[i]) = states[i];
    }

    // Resolve the predictied collision
    for (Manifold manifold : manifolds) {
        manifold.ApplyImpulse();
    }

    for (RigidBody* rbody : rbodies) {
        R4KIntegrate(rbody, dt);
    }

    for (Manifold manifold : manifolds) {
        manifold.PositionalCorrection();
    }
}

void PhysicsScene::DebugDraw(const Video::Camera& camera) const {
    gl33::glPointSize(4.0f);
    for (const Manifold& manifold : manifolds) {
        manifold.DebugDraw(camera);
    }
    gl33::glPointSize(1.0f);
}

void PhysicsScene::AddRigidBody(RigidBody* body) {
    rbodies.push_back(body);
    body->physics_scene = this;
}

RayContact PhysicsScene::CollideRay(const Ray& ray, const std::function<bool(RigidBody*)>& pred) const {
    RayContact closest{};
    for (RigidBody* rbody : rbodies) {
        if (pred(rbody)) {
            for (auto& bcircle : rbody->bounder_snapshot.circles) {
                closest = ClosestValidIntersection(closest, RayContact{rbody, Collide(bcircle, ray)});
            }
            for(auto& bbox : rbody->bounder_snapshot.boxes) {
                closest = ClosestValidIntersection(closest, RayContact{rbody, Collide(bbox, ray)});
            }
        }
    }
    return closest;
}
