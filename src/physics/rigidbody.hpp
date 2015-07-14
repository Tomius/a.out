#pragma once

#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include "boundingbox.hpp"
#include "boundingcircle.hpp"

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

    float staticFriction;
    float dynamicFriction;
    float restitution;

    std::vector<BoundingBox> bboxes;
    std::vector<BoundingCircle> bcircles;

    void Step(float dt);
    glm::mat3 GetMatrix() const;

    void AddBounder(BoundingBox);
    void AddBounder(BoundingCircle);

    void ApplyImpulse(glm::vec2 impulse);
    void ApplyForce(glm::vec2 force);

    // todo: how to apply torques not in the center of mass
    void ApplyTorqueImpulse(float impulse);
    void ApplyTorque(float torque);
};
