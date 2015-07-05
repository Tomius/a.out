#pragma once

#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

#include "rect.hpp"
#include "game_objects/game_object.hpp"

namespace Video
{

class Camera : public GameObjects::GameObject {
public:
    glm::vec2 lookAt;

    // Half of the window's height in world coordinates. The width will be
    // determined automatically from the window's ascpect ratio.
    float ysize = 1.0;

    glm::mat3 GetMatrix() const;
    Rect<float> GetRect() const;

    void ScreenResized(glm::ivec2 size) override;

private:
    glm::ivec2 window_size;
};

}
