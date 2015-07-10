#pragma once

#include <glm/vec2.hpp>
#include <vector>

#include "rigidbody.h"
#include "boundingbox.h"
#include "boundingcircle.h"

class Contact
{
public:
  glm::vec2 position;
  glm::vec2 normal;
  float penetration = 0;
};
 
class Manifold
{
public:
  std::vector<Contact> contacts;
  RigidBody* first;
  RigidBody* second;
};

Manifold Collide(RigidBody& first, RigidBody& second);

std::vector<Contact> Collide(BoundingCircle first, BoundingCircle second);
std::vector<Contact> Collide(BoundingBox first, BoundingBox second);
std::vector<Contact> Collide(BoundingCircle first, BoundingBox second);
std::vector<Contact> Collide(BoundingBox first, BoundingCircle second);
