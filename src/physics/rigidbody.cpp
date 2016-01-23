#include "rigidbody.hpp"
#include "math.hpp"

glm::vec2 kGravity{0, -9.81f};

void RigidBody::AddBounder(OrientedBoundingBox bbox) {
    bounder.boxes.push_back(bbox);
}

void RigidBody::AddBounder(BoundingCircle bcircle) {
    bounder.circles.push_back(bcircle);
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

void RigidBody::UpdateBounderCache() const {
    bounder_snapshot = WorldSpaceBounderSnapshot();

    for (auto& bbox : bounder.boxes) {
        bounder_snapshot.boxes.emplace_back(*this, bbox);
    }

    for (auto& bcircle : bounder.circles) {
        bounder_snapshot.circles.emplace_back(*this, bcircle);
    }

    UpdateAabb();
}

void RigidBody::UpdateAabb () const {
    glm::vec2& min = bounder_snapshot.aabb.min;
    glm::vec2& max = bounder_snapshot.aabb.max;

    min = max = position;
    for (auto& box : bounder_snapshot.boxes) {
        min = glm::min(min, box.min);
        max = glm::max(max, box.min + box.extent);
    }

     for (auto& circle : bounder_snapshot.circles) {
        min = glm::min(min, circle.center - glm::vec2(circle.radius));
        max = glm::max(max, circle.center + glm::vec2(circle.radius));
    }
}

glm::vec2 RigidBody::GetAcceleration(const PhysicsState& state) const {
    return kGravity;
}

float RigidBody::GetAngularAcceleration(const PhysicsState& state) const {
    return 0.0f;
}
