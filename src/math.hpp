#pragma once

#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

namespace Math {

constexpr float kEpsilon = 0.0001f;

template<typename T>
inline T Sqr(const T& value) {
    return value * value;
}

inline float Cross2D(glm::vec2 a, glm::vec2 b) {
    return a.x * b.y - a.y * b.x;
}

inline glm::vec2 Cross2D(glm::vec2 a, float b) {
    return {b * a.y, -b * a.x};
}

inline glm::vec2 Cross2D(float a, glm::vec2 b) {
    return -Cross2D(b, a);
}

inline glm::vec2 Rotate(glm::vec2 vector, float angle) {
    float c = cos(angle), s = sin(angle);
    return glm::mat2 { c, s,
                      -s, c} * vector;
}

inline glm::vec2 Rotate(glm::vec2 vector, glm::vec2 pivot, float angle) {
    return Rotate(vector - pivot, angle) + pivot;
}

}
