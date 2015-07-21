#include "bounding_box.hpp"
#include "rigidbody.hpp"
#include "math.hpp"


BoundingCircleSnapshot::BoundingCircleSnapshot(const RigidBody& rbody,
                                               const BoundingCircle& circle) {
    center = rbody.position + circle.center;
    radius = circle.radius;
}
