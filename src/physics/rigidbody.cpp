#include "rigidbody.hpp"

void RigidBody::AddBounder(BoundingBox bbox) {
    bboxes.push_back(bbox);
}

void RigidBody::AddBounder(BoundingCircle bcircle) {
    bcircles.push_back(bcircle);
}

glm::mat3 RigidBody::GetMatrix() const {
    float c = std::cos(orientation);
    float s = std::sin(orientation);

    // glm::mat3 ctor is column major!
    return glm::mat3 {
        c,           s,           0,
        -s,          c,           0,
        position.x,  position.y,  1
    };
}

void RigidBody::ApplyImpulse(glm::vec2 impulse) {
    velocity += impulse * inverse_mass;
}

void RigidBody::ApplyForce(glm::vec2 force) {
    acceleration += force * inverse_mass;
}

void RigidBody::ApplyTorqueImpulse(float impulse) {
    angular_velocity += impulse * inverse_inertia;
}

void RigidBody::ApplyTorque(float torque) {
    angular_velocity += torque * inverse_inertia;
}
