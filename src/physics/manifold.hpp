#pragma once

#include <glm/vec2.hpp>
#include <vector>

#include "rigidbody.hpp"
#include "boundingbox.hpp"
#include "boundingcircle.hpp"

class Contact
{
public:
  glm::vec2 normal;
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
};
