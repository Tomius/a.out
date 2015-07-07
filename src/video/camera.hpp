#pragma once

#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

#include "rect.hpp"
#include "game_objects/game_object.hpp"

namespace Video
{

class Camera : public GameObjects::GameObject {
public:
    enum class ViewportBehavior {
        kFixWidth, kFixHeight, kDynamic
    };

    glm::vec2 viewport_center;
    float viewport_size = 2.0f;
    ViewportBehavior viewport_behavior = ViewportBehavior::kDynamic;

    glm::mat3 GetMatrix() const;
    Rect<float> GetViewport() const;
    glm::vec2 GetViewportDimensions() const;
    void ScreenResized(glm::ivec2 size) override;

private:
    glm::ivec2 window_size;
};

}
