#include "bounding_box.hpp"
#include "rigidbody.hpp"
#include "math.hpp"


CachedBoundingCircle::CachedBoundingCircle(const RigidBody& rbody,
                                           const BoundingCircle& circle) {
    center = rbody.position + circle.center;
    radius = circle.radius;
}
