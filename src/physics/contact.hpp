#pragma once

#include <glm/vec2.hpp>
#include "physics/rigidbody.hpp"
#include "physics/ray.hpp"

class Contact
{
public:
    glm::vec2 position;
    glm::vec2 normal; // From first object to second
    float penetration = 0;
};

class RayContact
{
public:
    RigidBody* body = nullptr;
    RayTravelDistance r = 0;
};

constexpr bool IsValidRayContact(RayContact ray_contact) {
    return IsValidRayContact(ray_contact.r);
}

constexpr RayContact ClosestValidIntersection(RayContact a,
                                              RayContact b) {
    if (IsValidRayContact(a)) {
        if (IsValidRayContact(b)) {
            return a.r < b.r ? a : b;
        } else {
            return a;
        }
    } else {
        return b;
    }
}
