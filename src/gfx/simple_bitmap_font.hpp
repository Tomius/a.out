#pragma once

#include "gfx/rectangle.hpp"
#include "gfx/material/sprite_material.hpp"

namespace Gfx
{

class SimpleBitmapFont
{
public:
    SimpleBitmapFont(const std::string& filename, float width, float height);

    void Draw(const std::string& str, const glm::vec2& pos,
              const glm::mat3& mvp = glm::mat3{}) const;

private:
    mutable SpriteMaterial mat; // fuck it
    float width, height;
};

}
