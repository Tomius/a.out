#pragma once

#include <climits>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

namespace Math {

constexpr float kEpsilon = 0.0001f;

template<typename T>
constexpr bool EqualsWithEpsilon(const T& a, const T& b) {
    return std::abs(a - b) < kEpsilon;
}

inline glm::mat3 MoveForward(short num_layers = 1) {
    float bias = float(num_layers) / SHRT_MAX;
    return glm::mat3{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1.0f-bias
    };
}

inline glm::mat3 MoveForwardMax() {
    return glm::mat3{
        1, 0, 0,
        0, 1, 0,
        0, 0, 0
    };
}

inline glm::mat3 MoveBackward(short num_layers = 1) {
    return MoveForward(-num_layers);
}

inline glm::mat3 MoveBackwardMax() {
    return glm::mat3{
        1, 0, 0,
        0, 1, 0,
        0, 0, 2
    };
}

template<typename T>
inline T Sgn(const T& value) {
    if (std::abs(value) < kEpsilon) {
        return 0;
    } else if (0 < value) {
        return 1;
    } else {
        return -1;
    }
}

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

struct QuadraticSolitions {
    float roots[2]{};
    size_t num_roots = 0;
};

inline QuadraticSolitions SolveQuadratic (float a, float b, float c) {
    QuadraticSolitions solutions{};

    float det = b*b - 4*a*c;
    if (det < 0) {
        solutions.num_roots = 0;
    } else if (det == 0.0) {
        solutions.roots[0] = -b / (2*a);
        solutions.num_roots = 1;
    } else {
        solutions.roots[0] = (-b + sqrt(det)) / (2*a);
        solutions.roots[1] = (-b - sqrt(det)) / (2*a);
        solutions.num_roots = 2;
    }

    return solutions;
}

}
