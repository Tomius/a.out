#pragma once

#include <glm/vec2.hpp>

class RigidBody;

class BoundingBox {
public:
    glm::vec2 min;
    glm::vec2 max;
};

class OrientedBoundingBox {
public:
    glm::vec2 min;
    glm::vec2 extent;
    float orient = 0; // rotation around the min point
};


class OrientedBoundingBoxSnapshot : public OrientedBoundingBox {
public:
    glm::vec2 points[4]; // min (bottom left) is 0, bottom right is 1
    glm::vec2 normals[2]; // 0 is the "horizontal", both are towards max

    OrientedBoundingBoxSnapshot(const RigidBody& rbody,
                                const OrientedBoundingBox& obb);
};
