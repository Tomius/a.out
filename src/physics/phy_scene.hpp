#pragma once

#include "rigidbody.hpp"
#include "manifold.hpp"

class PhyScene {
public:
    static constexpr unsigned kIterationPerSec = 1000;
    static constexpr float kIterationInterval = 1.0f / kIterationPerSec;

    void Step(float dt);
    void AddRigidBody(RigidBody* body);
    void DebugDraw(const Video::Camera& camera) const;

private:
    std::vector<RigidBody*> rbodies;
    std::vector<Manifold> manifolds;

    void DoStep(float dt);
};
