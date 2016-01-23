#pragma once

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

class Ray {
public:
    const glm::vec2 origin;
    const glm::vec2 direction;

    Ray(glm::vec2 origin, glm::vec2 direction)
        : origin(origin), direction (glm::normalize(direction)) {}
};

using RayTravelDistance = float;

constexpr bool IsValidRayContact(RayTravelDistance r) {
    return r > 0;
}

constexpr RayTravelDistance ClosestValidIntersection(RayTravelDistance a,
                                                     RayTravelDistance b) {
    if (IsValidRayContact(a)) {
        if (IsValidRayContact(b)) {
            return std::min(a, b);
        } else {
            return a;
        }
    } else {
        return b;
    }
}
