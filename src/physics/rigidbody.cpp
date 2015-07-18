#include "rigidbody.hpp"
#include "math.hpp"

glm::vec2 kGravity{0, -9.81f};

void RigidBody::AddBounder(OrientedBoundingBox bbox) {
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

void RigidBody::ApplyImpulse(glm::vec2 impulse, glm::vec2 contactVector) {
    ApplyImpulse(impulse);
    ApplyTorqueImpulse(Math::Cross2D(contactVector, impulse));
}

void RigidBody::ApplyTorqueImpulse(float impulse) {
    angular_velocity += impulse * inverse_inertia;
}

void RigidBody::ApplyTorque(float torque) {
    angular_velocity += torque * inverse_inertia;
}

void RigidBody::UpdateBounderCache() const {
    cache_bboxes.clear();
    for (auto& bbox : bboxes) {
        cache_bboxes.emplace_back(*this, bbox);
    }

    cache_bcircles.clear();
    for (auto& bcircle : bcircles) {
        cache_bcircles.emplace_back(*this, bcircle);
    }
}

glm::vec2 RigidBody::GetAcceleration(const PhysicsState& state) const {
    return kGravity;
}

float RigidBody::GetAngularAcceleration(const PhysicsState& state) const {
    return 0.0f;
}
