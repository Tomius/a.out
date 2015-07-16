#pragma once

#include <glm/vec2.hpp>

class RigidBody;

class BoundingCircle {
public:
    glm::vec2 center;
    float radius = 0;
};


class CachedBoundingCircle : public BoundingCircle {
public:
    CachedBoundingCircle(const RigidBody& rbody, const BoundingCircle& circle);
};
