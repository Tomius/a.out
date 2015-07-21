#include "video/camera.hpp"

using Video::Camera;

glm::mat3 Camera::GetMatrix() const {
    glm::vec2 half_size = GetViewportDimensions() / 2.0f;
    const glm::vec2& lookAt = viewport_center;

    // mat3 ctor is column major!
    return glm::mat3{1/half_size.x,          0,                     0,
                     0,                      1/half_size.y,         0,
                     -lookAt.x/half_size.x,  -lookAt.y/half_size.y, 1};
}

Rect<float> Camera::GetViewport() const {
    glm::vec2 size = GetViewportDimensions();
    const glm::vec2& lookAt = viewport_center;

    return {lookAt.x - size.x/2, lookAt.y - size.y/2, size.x, size.y};
}

glm::vec2 Camera::GetViewportDimensions() const {
    glm::vec2 size;
    switch (viewport_behavior) {
        case ViewportBehavior::kFixWidth: {
            size.x = viewport_size;
            size.y = window_size.y * viewport_size / window_size.x;
            break;
        } case ViewportBehavior::kFixHeight: {
            size.x = window_size.x * viewport_size / window_size.y;
            size.y = viewport_size;
            break;
        } case ViewportBehavior::kDynamic: {
            float avg_window_size = (window_size.x + window_size.y) / 2.0f;
            size.x = window_size.x * viewport_size / avg_window_size;
            size.y = window_size.y * viewport_size / avg_window_size;
            break;
        }
    }
    return size;
}

void Camera::ScreenResized(glm::ivec2 size) {
    window_size = size;
}
