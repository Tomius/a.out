#pragma once

#include <glm/vec2.hpp>

class RigidBody;

class OrientedBoundingBox {
public:
    glm::vec2 min;
    glm::vec2 max;
    float orient = 0; // rotation around the min point
};


class CachedOrientedBoundingBox : public OrientedBoundingBox {
public:
    glm::vec2 extent;
    glm::vec2 points[4]; // min is 0, bottom right is 1
    glm::vec2 normals[2]; // 0 is "horizontal", both are towards max

    CachedOrientedBoundingBox(const RigidBody& rbody,
                              const OrientedBoundingBox& obb);
};
