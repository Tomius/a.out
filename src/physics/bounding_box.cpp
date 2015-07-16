#include <iostream>
#include "bounding_box.hpp"
#include "rigidbody.hpp"
#include "math.hpp"


CachedOrientedBoundingBox::CachedOrientedBoundingBox(const RigidBody& rbody,
                                                     const OrientedBoundingBox& obb)

{
    orient = obb.orient + rbody.orientation;
    min = Math::Rotate(obb.min, rbody.orientation) + rbody.position;
    extent = obb.max - obb.min;
    max = min + extent;

    points[0] = min;
    points[1] = min + Math::Rotate(glm::vec2{extent.x, 0}, orient);
    points[2] = min + Math::Rotate(glm::vec2{0, extent.y}, orient);
    points[3] = min + Math::Rotate(extent, orient);

    normals[0] = glm::normalize(points[1] - points[0]);
    normals[1] = glm::normalize(points[2] - points[0]);
}
