#include <iostream>
#include "manifold.hpp"
#include "gfx/pixel.hpp"
#include "gfx/line.hpp"
#include "gfx/material/color_material.hpp"


void Collide(std::vector<Contact>& contacts,
             BoundingCircle a,
             BoundingCircle b) {
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
        contact.normal = glm::vec2(1, 0);
        contact.position = a.center;
    }
}

void Collide(std::vector<Contact>& contacts,
             BoundingBox a,
             BoundingBox b) {
    glm::vec2 a_pos = (a.min + a.max) / 2.0f;
    glm::vec2 b_pos = (b.min + b.max) / 2.0f;

    // Vector from a to b
    glm::vec2 n = b_pos - a_pos;

    // Calculate half extents
    glm::vec2 a_extent = (a.max - a.min) / 2.0f;
    glm::vec2 b_extent = (b.max - b.min) / 2.0f;

    // Calculate overlap
    glm::vec2 overlap = a_extent + b_extent - glm::abs(n);

    if (overlap.x > 0 && overlap.y > 0) {
        Contact contact;
        // Find out which axis is axis of least penetration
        // TODO: this approach only works if dt is small enough
        if (overlap.x < overlap.y) {
            if (n.x < 0)
                contact.normal = glm::vec2(-1, 0);
            else
                contact.normal = glm::vec2(1, 0);
            contact.penetration = overlap.x;

            float sgn = b_pos.x > a_pos.x ? 1.0f : -1.0f;
            float contact_x = a_pos.x + sgn * a_extent.x;
            contact.position = glm::vec2{contact_x, std::max(
                a_pos.y - a_extent.y, b_pos.y - b_extent.y)};
            contacts.push_back(contact);
            contact.position = glm::vec2{contact_x, std::min(
                a_pos.y + a_extent.y, b_pos.y + b_extent.y)};
            contacts.push_back(contact);
        } else {
            if (n.y < 0)
                contact.normal = glm::vec2(0, -1);
            else
                contact.normal = glm::vec2(0, 1);
            contact.penetration = overlap.y;

            float sgn = b_pos.y > a_pos.y ? 1.0f : -1.0f;
            float contact_y = a_pos.y + sgn * a_extent.y;
            contact.position = glm::vec2{std::max(a_pos.x - a_extent.x,
                b_pos.x - b_extent.x), contact_y};
            contacts.push_back(contact);
            contact.position = glm::vec2{std::min(a_pos.x + a_extent.x,
                b_pos.x + b_extent.x), contact_y};
            contacts.push_back(contact);
        }
    }
}

void Collide(std::vector<Contact>& contacts,
             BoundingBox a,
             BoundingCircle b) {
    // precull
    if (b.center.x < a.min.x - b.radius ||
        b.center.y < a.min.y - b.radius ||
        b.center.x > a.max.x + b.radius ||
        b.center.y > a.max.y + b.radius) {
        return;
    }

    glm::vec2 a_pos = (a.min + a.max) / 2.0f;

    // Vector from a to b;
    glm::vec2 n = b.center - a_pos;

    // Calculate half extents along each axis
    glm::vec2 extent = (a.max - a.min) / 2.0f;

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
    float r = b.radius;

    // Early out of the radius is shorter than distance to closest point and
    // Circle not inside the AABB
    if (d > r * r && !inside)
        return;

    d = sqrt(d);

    contacts.emplace_back();
    Contact& contact = contacts.back();
    // Collision normal needs to be flipped to point
    // outside if circle was inside the AABB
    contact.normal = (inside ? -normal : normal) / d;
    contact.penetration = r - d;
    contact.position = b.center - b.radius * glm::normalize(contact.normal);
}

void Collide(std::vector<Contact>& contacts,
             BoundingCircle a,
             BoundingBox b) {
    size_t old_size = contacts.size();
    Collide(contacts, b, a);
    if (contacts.size() > old_size) { // If there was a contact
        // The normal has to go from the a object to the b
        contacts.back().normal *= -1.0f;
    }
}

Manifold::Manifold(RigidBody* a, RigidBody* b)
        : first(a), second(b) {
    if (a == b || a->inverse_mass + b->inverse_mass == 0.0f) {
        return;
    }

    for (auto& x : a->bcircles) {
        for (auto& y : b->bcircles) {
            Collide(contacts, x, y);
        }
        for(auto& y : b->bboxes) {
            Collide(contacts, x, y);
        }
    }
    for(auto& x : a->bboxes) {
        for (auto& y : b->bcircles) {
            Collide(contacts, x, y);
        }
        for(auto& y : b->bboxes) {
            Collide(contacts, x, y);
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
        Gfx::Pixel::Draw(contact.position, red, camera.GetMatrix());
        Gfx::Line::Draw(contact.position,
                        contact.position + contact.normal*0.25f,
                        green, camera.GetMatrix());
    }
}
