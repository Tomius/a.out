#pragma once

#include <vector>

#include "physics/contact.hpp"
#include "physics/rigidbody.hpp"
#include "video/camera.hpp"

class Manifold
{
private:
    std::vector<Contact> contacts;
    RigidBody* first;
    RigidBody* second;

public:
    Manifold(RigidBody* first, RigidBody* second);
    void ApplyImpulse();
    void PositionalCorrection();
    void DebugDraw(const Video::Camera& camera) const;
    bool IsEmpty() const { return contacts.empty(); }
};
