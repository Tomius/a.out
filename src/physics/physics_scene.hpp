#pragma once

#include "ray.hpp"
#include "rigidbody.hpp"
#include "manifold.hpp"

inline bool AcceptAll(RigidBody*) {
    return true;
}

class PhysicsScene {
public:
    static constexpr unsigned kIterationPerSec = 1000;
    static constexpr float kIterationInterval = 1.0f / kIterationPerSec;

    void Step(float dt);
    void AddRigidBody(RigidBody* body);
    void DebugDraw(const Video::Camera& camera) const;

    RayContact CollideRay(const Ray& ray,
                          const std::function<bool(RigidBody*)>& pred = AcceptAll) const;

private:
    std::vector<RigidBody*> rbodies;
    std::vector<Manifold> manifolds;

    void DoStep(float dt);
};
