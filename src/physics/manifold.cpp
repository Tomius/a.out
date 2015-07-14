#include <iostream>
#include "manifold.hpp"
#include "gfx/pixel.hpp"
#include "gfx/line.hpp"
#include "gfx/material/color_material.hpp"


Contact Collide(BoundingCircle first, BoundingCircle second) {
    Contact contact;

    // Vector from first to second
    glm::vec2 n = second.center - first.center;

    float r = first.radius + second.radius;
    float sqr_r = r*r;

    if (glm::dot(n, n) > sqr_r)
        return contact;

    // Circles have collided, now compute manifold
    float d = glm::length(n);

    // If distance between circles is not zero
    if (d != 0) {
        // Distance is difference between radius and distance
        contact.penetration = r - d;
        // the normal is n normalized
        contact.normal = n / d;
        contact.contacts[0] = first.center + contact.normal*first.radius;
    } else { // Circles are on same position
        // Choose random (but consistent) values
        contact.penetration = first.radius;
        contact.normal = glm::vec2(1, 0);
        contact.contacts[0] = first.center;
    }
    contact.contact_count = 1;

    return contact;
}

Contact Collide(BoundingBox first, BoundingBox second) {
    Contact contact;
    glm::vec2 first_pos = (first.min + first.max) / 2.0f;
    glm::vec2 second_pos = (second.min + second.max) / 2.0f;

    // Vector from first to second
    glm::vec2 n = second_pos - first_pos;

    // Calculate half extents
    glm::vec2 first_extent = (first.max - first.min) / 2.0f;
    glm::vec2 second_extent = (second.max - second.min) / 2.0f;

    // Calculate overlap
    glm::vec2 overlap = first_extent + second_extent - glm::abs(n);

    if (overlap.x > 0 && overlap.y > 0) {
        // Find out which axis is axis of least penetration
        // TODO: this approach only works if dt is small enough
        if (overlap.x < overlap.y) {
            if (n.x < 0)
                contact.normal = glm::vec2(-1, 0);
            else
                contact.normal = glm::vec2(1, 0);
            contact.penetration = overlap.x;

            float sgn = second_pos.x > first_pos.x ? 1.0f : -1.0f;
            float contact_x = first_pos.x + sgn * first_extent.x;
            contact.contacts[0] = glm::vec2{contact_x, std::max(
                first_pos.y - first_extent.y, second_pos.y - second_extent.y)};
            contact.contacts[1] = glm::vec2{contact_x, std::min(
                first_pos.y + first_extent.y, second_pos.y + second_extent.y)};
        } else {
            if (n.y < 0)
                contact.normal = glm::vec2(0, -1);
            else
                contact.normal = glm::vec2(0, 1);
            contact.penetration = overlap.y;

            float sgn = second_pos.y > first_pos.y ? 1.0f : -1.0f;
            float contact_y = first_pos.y + sgn * first_extent.y;
            contact.contacts[0] = glm::vec2{std::max(first_pos.x - first_extent.x,
                second_pos.x - second_extent.x), contact_y};
            contact.contacts[1] = glm::vec2{std::min(first_pos.x + first_extent.x,
                second_pos.x + second_extent.x), contact_y};
        }
        contact.contact_count = 2;
    }

    return contact;
}

Contact Collide(BoundingBox first, BoundingCircle second) {
    Contact contact;

    // precull
    if (second.center.x < first.min.x - second.radius ||
        second.center.y < first.min.y - second.radius ||
        second.center.x > first.max.x + second.radius ||
        second.center.y > first.max.y + second.radius) {
        return contact;
    }

    glm::vec2 first_pos = (first.min + first.max) / 2.0f;

    // Vector from first to second;
    glm::vec2 n = second.center - first_pos;

    // Calculate half extents along each axis
    glm::vec2 extent = (first.max - first.min) / 2.0f;

    // Closest point on the box to center of the circle
    // (Clamp direction vector to edges of the AABB)
    glm::vec2 closest = glm::clamp(n, -extent, extent);

    bool inside = false;

    // Circle is inside the AABB, so we need to clamp the circle's center
    // to the closest edge
    if (n == closest) {
        inside = true;

        // Find closest axis
        if (std::abs(n.x) > std::abs(n.y)) {
            // Clamp to closest extent
            if (closest.x > 0)
                closest.x = extent.x;
            else
                closest.x = -extent.x;
        } else { // y axis is shorter
            // Clamp to closest extent
            if (closest.y > 0)
                closest.y = extent.y;
            else
                closest.y = -extent.y;
        }
    }

    glm::vec2 normal = n - closest;
    float d = glm::dot(normal, normal);
    float r = second.radius;

    // Early out of the radius is shorter than distance to closest point and
    // Circle not inside the AABB
    if (d > r * r && !inside)
        return contact;

    d = sqrt(d);

    // Collision normal needs to be flipped to point outside if circle was
    // inside the AABB
    contact.normal = (inside ? -normal : normal) / d;
    contact.penetration = r - d;
    contact.contacts[0] = second.center - second.radius * glm::normalize(contact.normal);
    contact.contact_count = 1;

    return contact;
}

Contact Collide(BoundingCircle first, BoundingBox second) {
    Contact contact = Collide(second, first);
    contact.normal = -contact.normal;
    return contact;
}

Manifold::Manifold(RigidBody* first, RigidBody* second)
        : first(first), second(second) {
    if (first == second || first->inverse_mass + second->inverse_mass == 0.0f) {
        return;
    }

    for (auto& a : first->bcircles) {
        for (auto& b : second->bcircles) {
            Contact c = Collide(a, b);
            if (c.contact_count > 0) { contacts.push_back(c); }
        }
        for(auto& b : second->bboxes) {
            Contact c = Collide(a, b);
            if (c.contact_count > 0) { contacts.push_back(c); }
        }
    }
    for(auto& a : first->bboxes) {
        for (auto& b : second->bcircles) {
            Contact c = Collide(a, b);
            if (c.contact_count > 0) { contacts.push_back(c); }
        }
        for(auto& b : second->bboxes) {
            Contact c = Collide(a, b);
            if (c.contact_count > 0) { contacts.push_back(c); }
        }
    }
}

void Manifold::ApplyImpulse() {
    for (auto& contact : contacts) {
        // ========================= Base impulse ==============================

        // Calculate relative velocity
        glm::vec2 rv = second->velocity - first->velocity;

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = glm::dot(rv, contact.normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0)
            continue ;

        // Calculate restitution
        float e = std::min(first->restitution, second->restitution);

        // Calculate impulse ratio
        float j = -(1 + e) * velAlongNormal;
        j /= first->inverse_mass + second->inverse_mass;
        j /= contacts.size();

        // Apply impulse
        glm::vec2 impulse = j * contact.normal;
        first->ApplyImpulse(-impulse);
        second->ApplyImpulse(impulse);

        // ======================= Frition impulse =============================

        // Recalculate relative velocity
        rv = second->velocity - first->velocity;

        // Solve for the tangent vector
        glm::vec2 tangent = rv - glm::dot(rv, contact.normal) * contact.normal;
        float d = glm::dot(tangent, tangent);
        if (d < 0.0001f) { continue; }
        tangent /= std::sqrt(d);

        // Solve for magnitude to apply along the friction vector
        float jt = -glm::dot(rv, tangent);
        jt /= first->inverse_mass + second->inverse_mass;
        jt /= contacts.size();

        // Don't apply tiny friction impulses
        if (std::abs(jt) < 0.0001f) { continue; }

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
        first->ApplyImpulse(-frictionImpulse);
        second->ApplyImpulse(frictionImpulse);
    }
}

void Manifold::DebugDraw(const Video::Camera& camera) const {
    Gfx::ColorMaterial red{glm::vec4{0, 1, 0, 1}};
    Gfx::ColorMaterial green{glm::vec4{1, 0, 0, 1}};

    for (const Contact& contact : contacts) {
        for (unsigned i = 0; i < contact.contact_count; i++) {
            Gfx::Pixel::Draw(contact.contacts[i], red, camera.GetMatrix());
            Gfx::Line::Draw(contact.contacts[i],
                            contact.contacts[i] + contact.normal*0.25f,
                            green, camera.GetMatrix());
        }
    }
}
