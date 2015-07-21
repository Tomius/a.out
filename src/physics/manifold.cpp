#include <iostream>
#include "math.hpp"
#include "manifold.hpp"
#include "bounding_box.hpp"
#include "bounding_circle.hpp"

#include "gfx/pixel.hpp"
#include "gfx/line.hpp"
#include "gfx/material/color_material.hpp"

static void Collide(std::vector<Contact>& contacts,
                    const BoundingCircleSnapshot& a,
                    const BoundingCircleSnapshot& b) {
    // Vector from a to b
    glm::vec2 n = b.center - a.center;

    float r = a.radius + b.radius;
    float sqr_r = r*r;

    if (glm::dot(n, n) > sqr_r)
        return;

    // Circles have collided, now compute manifold
    float d = glm::length(n);

    contacts.emplace_back();
    Contact& contact = contacts.back();
    // If distance between circles is not zero
    if (d != 0) {
        // Distance is difference between radius and distance
        contact.penetration = r - d;
        // the normal is n normalized
        contact.normal = n / d;
        contact.position = a.center + contact.normal*a.radius;
    } else { // Circles are on same position
        // Choose random (but consistent) values
        contact.penetration = a.radius;
        contact.normal = glm::vec2{1, 0};
        contact.position = a.center;
    }
}

static void CollideOneWay(std::vector<Contact>& contacts,
                          const OrientedBoundingBoxSnapshot& a,
                          const OrientedBoundingBoxSnapshot& b,
                          bool flip_normals) {
    // half extent
    glm::vec2 half_extent = a.extent / 2.0f;
    for (const glm::vec2& point : b.points) {
        float min_penetration;
        glm::vec2 min_normal;
        bool sat_fail = false;

        for (int i = 0; i < 2; i++) {
            float x = glm::dot(a.normals[i], point - a.min);
            // the point's projection is inside the a's projection
            if (0 < x && x < 2*half_extent[i]) {
                float dist_from_center = half_extent[i] - x;
                float penetration = half_extent[i] - std::abs(dist_from_center);
                if (i == 0 || penetration < min_penetration) {
                    min_penetration = penetration;
                    min_normal = dist_from_center < 0 ? a.normals[i] : -a.normals[i];
                }
            } else {
                // if the point is not inside of one projections,
                // then the point is surely not inside a.
                sat_fail = true;
                break;
            }
        }

        if (sat_fail) { continue; }

        contacts.emplace_back();
        Contact& contact = contacts.back();
        contact.position = point;
        contact.penetration = min_penetration;
        contact.normal = flip_normals ? -min_normal : min_normal;
    }
}

static void Collide(std::vector<Contact>& contacts,
                    const OrientedBoundingBoxSnapshot& a,
                    const OrientedBoundingBoxSnapshot& b) {
    CollideOneWay(contacts, a, b, false);
    CollideOneWay(contacts, b, a, true);
}

static void Collide(std::vector<Contact>& contacts,
                    const OrientedBoundingBoxSnapshot& a,
                    const BoundingCircleSnapshot& b,
                    bool flip_normals = false) {

    // rotate the "world", so that a becomes an AABB
    glm::vec2 b_rot_center = Math::Rotate(b.center, a.min, -a.orient);

    // precull
    if (b_rot_center.x < a.min.x - b.radius ||
        b_rot_center.y < a.min.y - b.radius ||
        b_rot_center.x > a.min.x + a.extent.x + b.radius ||
        b_rot_center.y > a.min.y + a.extent.y + b.radius) {
        return;
    }

    glm::vec2 half_extent = a.extent / 2.0f;
    glm::vec2 a_center = a.min + half_extent;

    // Vector from a to b;
    glm::vec2 n = b_rot_center - a_center;

    // Closest point on the box to center of the circle
    // (Clamp direction vector to edges of the AABB)
    glm::vec2 closest = glm::clamp(n, -half_extent, half_extent);

    bool inside = false;

    // Circle is inside the AABB, so we need to clamp the circle's center
    // to the closest edge
    if (n == closest) {
        inside = true;

        // Find closest axis
        if (std::abs(n.x) > std::abs(n.y)) {
            // Clamp to closest half_extent
            if (closest.x > 0)
                closest.x = half_extent.x;
            else
                closest.x = -half_extent.x;
        } else { // y axis is shorter
            // Clamp to closest half_extent
            if (closest.y > 0)
                closest.y = half_extent.y;
            else
                closest.y = -half_extent.y;
        }
    }

    glm::vec2 normal = n - closest;
    float d = glm::dot(normal, normal);
    float r = b.radius;

    // Early out of the radius is shorter than distance to closest point and
    // Circle not inside the AABB
    if (d > r * r && !inside)
        return;

    d = sqrt(d);

    contacts.emplace_back();
    Contact& contact = contacts.back();
    if (d < Math::kEpsilon) {
        // if normal is nullvector, pick a random, but consistent one
        contact.normal = glm::vec2{1, 0};
        d = 1;
    } else {
        // Collision normal needs to be flipped to point
        // outside if circle is inside the AABB
        contact.normal = (inside ? -normal : normal) / d;
    }

    // rotate the "world" back
    contact.normal = Math::Rotate(contact.normal, a.orient);
    contact.penetration = r - d;
    // original center, not the rotated one
    contact.position = b.center - b.radius * glm::normalize(contact.normal);

    // flip normals only after contact position is calculated
    if (flip_normals) {
        contact.normal *= -1.0;
    }
}

static void Collide(std::vector<Contact>& contacts,
                    const BoundingCircleSnapshot& a,
                    const OrientedBoundingBoxSnapshot& b) {
    Collide(contacts, b, a, true);
}

Manifold::Manifold(RigidBody* a, RigidBody* b)
        : first(a), second(b) {
    if (a == b || a->inverse_mass + b->inverse_mass == 0.0f) {
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
    const float kSlop = 0.05f; // Penetration allowance
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
