#pragma once

#include <glm/vec2.hpp>

class RigidBody;

class BoundingCircle {
public:
    glm::vec2 center;
    float radius = 0;
};


class BoundingCircleSnapshot : public BoundingCircle {
public:
    BoundingCircleSnapshot(const RigidBody& rbody, const BoundingCircle& circle);
};
