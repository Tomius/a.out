#pragma once

#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

namespace Math {

constexpr float kEpsilon = 0.0001f;

inline glm::vec2 Rotate(glm::vec2 vector, float angle) {
    float c = cos(angle), s = sin(angle);
    return glm::mat2 { c, s,
                      -s, c} * vector;
}

inline glm::vec2 Rotate(glm::vec2 vector, glm::vec2 pivot, float angle) {
    return Rotate(vector - pivot, angle) + pivot;
}

}
