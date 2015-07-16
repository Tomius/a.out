#pragma once

#include <glm/vec2.hpp>
#include <vector>

#include "rigidbody.hpp"
#include "video/camera.hpp"

class Contact
{
public:
    glm::vec2 position;
    glm::vec2 normal; // From first object to second
    float penetration = 0;
};

class Manifold
{
private:
    std::vector<Contact> contacts;
    RigidBody* first;
    RigidBody* second;

public:
    Manifold(RigidBody* first, RigidBody* second);
    void ApplyImpulse();
    void DebugDraw(const Video::Camera& camera) const;
};
