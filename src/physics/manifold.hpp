#pragma once

#include <glm/vec2.hpp>
#include <vector>

#include "rigidbody.hpp"
#include "boundingbox.hpp"
#include "boundingcircle.hpp"
#include "video/camera.hpp"

class Contact
{
public:
    glm::vec2 contacts[2];
    unsigned contact_count = 0;
    glm::vec2 normal; // From first to second
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
