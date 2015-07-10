#pragma once

#include <iostream>
#include <glm/vec2.hpp>

namespace Physics
{

glm::dvec2 kGravity{0, -9.81f};

class PhysicsState {
public:
    double mass = 1.0;
    glm::dvec2 pos, speed;

    void Step(double dt) {
        speed += GetAccel() * dt;
        pos += speed * dt;

        varying_accel = glm::dvec2(0);

        // todo
        if (pos.y < 0) {
            pos.y = 0;
            if (speed.y < 0) {
                speed.y = 0;
            }
        }
    }

    void ApplyImpulse(glm::dvec2 impulse) {
        speed += impulse / mass;
    }

    void ApplyForce(glm::dvec2 force) {
        varying_accel += force / mass;
    }

    void ApplyForceField(glm::dvec2 force) {
        constant_accel += force / mass;
    }

    glm::dvec2 GetAccel() const {
        return constant_accel + varying_accel;
    }

private:
    glm::dvec2 constant_accel, varying_accel;
};

}
