#include <iostream>
#include "manifold.hpp"

bool Collide(Contact* contact, BoundingCircle first, BoundingCircle second) {
    // Vector from first to second
    glm::vec2 n = second.center - first.center;

    float r = first.radius + second.radius;
    float sqr_r = r*r;

    if (glm::dot(n, n) > sqr_r)
        return false;

    // Circles have collided, now compute manifold
    float d = glm::length(n);

    // If distance between circles is not zero
    if (d != 0) {
        // Distance is difference between radius and distance
        contact->penetration = r - d;
        // the normal is n normalized
        contact->normal = n / d;
        return true;
    } else { // Circles are on same position
        // Choose random (but consistent) values
        contact->penetration = first.radius;
        contact->normal = glm::vec2(1, 0);
        return true;
    }
}

bool Collide(Contact* contact, BoundingBox first, BoundingBox second) {
    glm::vec2 first_pos = (first.min + first.max)/2.0f;
    glm::vec2 second_pos = (second.min + second.max)/2.0f;

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
        if (overlap.x > overlap.y) {
            if (n.x < 0)
                contact->normal = glm::vec2(-1, 0);
            else
                contact->normal = glm::vec2(1, 0);
            contact->penetration = overlap.x;
        } else {
            if (n.y < 0)
                contact->normal = glm::vec2(0, -1);
            else
                contact->normal = glm::vec2(0, 1);
            contact->penetration = overlap.y;
        }
        return true;
    } else {
        return false;
    }
}

bool Collide(Contact* contact, BoundingBox first, BoundingCircle second) {
    glm::vec2 first_pos = (first.min + first.max)/2.0f;

    // Vector from first to second;
    glm::vec2 n = second.center - first_pos;

    // Closest point on the box to center of the circle
    glm::vec2 closest = n;

    // Calculate half extents along each axis
    glm::vec2 extent = (first.max - first.min) / 2.0f;

    // Clamp point to edges of the AABB
    closest = glm::clamp(-extent, extent, closest);

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
        return false;

    d = sqrt(d);

    // Collision normal needs to be flipped to point outside if circle was
    // inside the AABB
    contact->normal = (inside ? -normal : normal) / d;
    contact->penetration = r + d;

    return true;
}

bool Collide(Contact* contact, BoundingCircle first, BoundingBox second) {
    bool collision = Collide(contact, second, first);
    contact->normal *= -1.0f;
    return collision;
}

Manifold::Manifold(RigidBody* first, RigidBody* second)
        : first(first), second(second) {
    if (first == second || first->inverse_mass + second->inverse_mass == 0.0f) {
        return;
    }

    Contact c;
    for (auto& bc : first->bcircles) {
        for (auto& sbc : second->bcircles) {
            if (Collide(&c, bc, sbc)) {
                contacts.push_back(c);
            }
        }
        for(auto& sbc : second->bboxes) {
            if (Collide(&c, bc, sbc)) {
                contacts.push_back(c);
            }
        }
    }
    for(auto& bc : first->bboxes) {
        for (auto& sbc : second->bcircles) {
            if (Collide(&c, bc, sbc)) {
                contacts.push_back(c);
            }
        }
        for(auto& sbc : second->bboxes) {
            if (Collide(&c, bc, sbc)) {
                contacts.push_back(c);
            }
        }
    }
}


void Manifold::ApplyImpulse() {
    for (auto& contact : contacts) {
        // Calculate relative velocity
        glm::vec2 rv = second->velocity - first->velocity;

        // Calculate relative velocity in terms of the normal direction
        float velAlongNormal = glm::dot(rv, contact.normal);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0)
            continue;

        // Calculate restitution
        float e = std::min(first->restitution, second->restitution);

        // Calculate impulse ratio
        float j = -(1 + e) * velAlongNormal;
        j /= first->inverse_mass + second->inverse_mass;

        // Apply impulse
        glm::vec2 impulse = j * contact.normal;
        first->ApplyImpulse(-impulse);
        second->ApplyImpulse(impulse);
    }
}

// void Manifold::PositionalCorrection() {
//     for (auto& contact : contacts) {
//         const float k_slop = 0.05f; // Penetration allowance
//         const float percent = 0.4f; // Penetration percentage to correct
//         glm::vec2 correction = (std::max(contact.penetration - k_slop, 0.0f)
//                                 / (first->inverse_mass + second->inverse_mass))
//                                 * contact.normal * percent;
//         first->position -= correction * first->inverse_mass;
//         second->position += correction * second->inverse_mass;
//     }
// }
