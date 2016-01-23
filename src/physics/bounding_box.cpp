#include <iostream>
#include "bounding_box.hpp"
#include "rigidbody.hpp"
#include "math.hpp"

OrientedBoundingBoxSnapshot::OrientedBoundingBoxSnapshot(const RigidBody& rbody,
                                                         const OrientedBoundingBox& obb)

{
    orient = obb.orient + rbody.orientation;
    min = Math::Rotate(obb.min, rbody.orientation) + rbody.position;
    extent = obb.extent;

    points[0] = min;
    points[1] = min + Math::Rotate(glm::vec2{extent.x, 0}, orient);
    points[2] = min + Math::Rotate(extent, orient);
    points[3] = min + Math::Rotate(glm::vec2{0, extent.y}, orient);

    normals[0] = glm::normalize(points[1] - points[0]);
    normals[1] = glm::normalize(points[3] - points[0]);
}
