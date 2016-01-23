#include <iostream>
#include "math.hpp"
#include "manifold.hpp"
#include "collision.hpp"

#include "gfx/pixel.hpp"
#include "gfx/line.hpp"
#include "gfx/material/color_material.hpp"

Manifold::Manifold(RigidBody* a, RigidBody* b)
        : first(a), second(b) {
    if (a == b || a->inverse_mass + b->inverse_mass == 0.0f ||
        !CanCollide(a->bounder_snapshot.aabb, b->bounder_snapshot.aabb)) {
        return;
    }

    for (auto& x : a->bounder_snapshot.circles) {
        for (auto& y : b->bounder_snapshot.circles) {
            Collide(contacts, x, y);
        }
        for(auto& y : b->bounder_snapshot.boxes) {
            Collide(contacts, x, y);
        }
    }
    for(auto& x : a->bounder_snapshot.boxes) {
        for (auto& y : b->bounder_snapshot.circles) {
            Collide(contacts, x, y);
        }
        for(auto& y : b->bounder_snapshot.boxes) {
            Collide(contacts, x, y);
        }
    }
}

void Manifold::ApplyImpulse() {
    for (auto& contact : contacts) {
        // ========================= Base impulse ==============================

        // Vector from the objects center of mass to the contact point
        glm::vec2 rfirst = contact.position - first->position;
        glm::vec2 rsecond = contact.position - second->position;

        // Calculate relative velocity
        glm::vec2 rv =
             second->velocity + Math::Cross2D(second->angular_velocity, rsecond)
            -first->velocity  - Math::Cross2D(first->angular_velocity,  rfirst);

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = glm::dot(rv, contact.normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0) { continue; }

        // Calculate restitution
        float e = std::min(first->restitution, second->restitution);

        // Calc inverse mass sum
        float rfcn = Math::Cross2D(rfirst, contact.normal);
        float rscn = Math::Cross2D(rsecond, contact.normal);
        float invMassSum =
            first->inverse_mass + second->inverse_mass
            + Math::Sqr(rfcn) * first->inverse_inertia
            + Math::Sqr(rscn) * second->inverse_inertia;

        // Calculate impulse ratio
        float j = -(1 + e) * velAlongNormal;
        j /= invMassSum;
        j /= contacts.size();

        // Apply impulse
        glm::vec2 impulse = j * contact.normal;
        first->ApplyImpulse(-impulse, rfirst);
        second->ApplyImpulse(impulse, rsecond);

        // ======================= Rolling friction =============================

        float rf = std::sqrt(first->rolling_friction * second->rolling_friction);
        first->ApplyTorqueImpulse(rf * -Math::Sgn(first->angular_velocity) * j);
        second->ApplyTorqueImpulse(rf * -Math::Sgn(second->angular_velocity) * j);

        // ======================= Frition impulse =============================

        // Recalculate relative velocity
        rv =  second->velocity + Math::Cross2D(second->angular_velocity, rsecond)
             -first->velocity  - Math::Cross2D(first->angular_velocity,  rfirst);

        // Solve for the tangent vector
        glm::vec2 tangent = rv - glm::dot(rv, contact.normal) * contact.normal;
        float d = glm::dot(tangent, tangent);
        if (d < Math::kEpsilon) { continue; }
        tangent /= std::sqrt(d);

        // Solve for magnitude to apply along the friction vector
        float jt = -glm::dot(rv, tangent);
        jt /= invMassSum;
        jt /= contacts.size();

        // Don't apply tiny friction impulses
        if (std::abs(jt) < Math::kEpsilon) { continue; }

        // Calculate mixed static and dynamic friction
        float sf = std::sqrt(first->static_friction * second->static_friction);
        float df = std::sqrt(first->dynamic_friction * second->dynamic_friction);

        // Clamp magnitude of friction and create impulse vector
        glm::vec2 frictionImpulse;
        if (std::abs(jt) < j * sf) {
          frictionImpulse = tangent * jt;
        } else {
          frictionImpulse = tangent * -j * df;
        }

        // Apply
        first->ApplyImpulse(-frictionImpulse, rfirst);
        second->ApplyImpulse(frictionImpulse, rsecond);
    }
}

void Manifold::PositionalCorrection() {
    const float kSlop = 0.001f; // Penetration allowance
    const float kPercent = 0.4f; // Penetration percentage to correct

    size_t contacts_to_correct_num = 0;
    for (const Contact& contact : contacts) {
        if (contact.penetration > kSlop) { contacts_to_correct_num++; }
    }

    if (contacts_to_correct_num == 0) {
        return;
    }

    for (const Contact& contact : contacts) {
        float correction_value = std::max(contact.penetration - kSlop, 0.0f);
        float sum_inv_mass = first->inverse_mass + second->inverse_mass;
        correction_value *= kPercent / contacts_to_correct_num;
        glm::vec2 correction = correction_value * contact.normal;
        first->position -= correction * (first->inverse_mass / sum_inv_mass);
        second->position += correction * (second->inverse_mass / sum_inv_mass);
    }
}

void Manifold::DebugDraw(const Video::Camera& camera) const {
    Gfx::ColorMaterial red{glm::vec4{0, 1, 0, 1}};
    Gfx::ColorMaterial green{glm::vec4{1, 0, 0, 1}};

    for (const Contact& contact : contacts) {
        Gfx::Pixel::Draw(contact.position, red, camera.GetMatrix());
        Gfx::Line::Draw(contact.position,
                        contact.position + contact.normal*0.25f,
                        green, camera.GetMatrix());
    }
}
