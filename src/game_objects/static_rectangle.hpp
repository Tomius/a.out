#pragma once

#include <glm/vec4.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "game_objects/game_object.hpp"

namespace GameObjects
{

class StaticRectangle : public GameObject
{
public:
    StaticRectangle(Rect<float> rect, glm::vec4 const& color);

    void Draw() override;

private:
    Rect<float> rect;
    glm::vec4 color;
};

}
