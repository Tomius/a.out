#include "camera.hpp"

using Video::Camera;

glm::mat3 Camera::GetMatrix() const {
    glm::vec2 size{window_size.x * ysize / window_size.y, ysize};

    // mat3 ctor is column major!
    return glm::mat3{1/size.x,          0,               0,
                     0,                 1/size.y,        0,
                     -lookAt.x/size.x, -lookAt.y/size.y, 1};
}

Rect<float> Camera::GetRect() const {
    glm::vec2 size{window_size.x * ysize / window_size.y, ysize};
    return {lookAt.x - size.x, lookAt.y - size.y, 2*size.x, 2*size.y};
}

void Camera::ScreenResized(glm::ivec2 size) {
    window_size = size;
}
