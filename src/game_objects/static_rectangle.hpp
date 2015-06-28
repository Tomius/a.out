#ifndef UUID_407F9AC5_BF7C_4D03_A71E_DBB039E16F76
#define UUID_407F9AC5_BF7C_4D03_A71E_DBB039E16F76
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

#endif
