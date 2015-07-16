#pragma once

#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include "bounding_box.hpp"
#include "bounding_circle.hpp"

class RigidBody {
public:
    float inverse_mass = 0; // infitine mass == environment
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;

    float inverse_inertia = 0;
    float orientation = 0; // in radians
    float angular_velocity = 0;
    float angular_acceleration = 0;

    float static_friction = 0.5f;
    float dynamic_friction = 0.3f;
    float restitution = 0.4f;

    std::vector<OrientedBoundingBox> bboxes;
    std::vector<BoundingCircle> bcircles;

    mutable std::vector<CachedOrientedBoundingBox> cache_bboxes;
    mutable std::vector<CachedBoundingCircle> cache_bcircles;

    glm::mat3 GetMatrix() const;

    void AddBounder(OrientedBoundingBox);
    void AddBounder(BoundingCircle);

    void ApplyImpulse(glm::vec2 impulse);
    void ApplyImpulse(glm::vec2 impulse, glm::vec2 contactVector);
    void ApplyForce(glm::vec2 force);
    void ApplyForce(glm::vec2 force, glm::vec2 contactVector);

    void ApplyTorqueImpulse(float impulse);
    void ApplyTorque(float torque);

    void UpdateBounderCache() const;
};
