#include "game_objects/static_rectangle.hpp"
#include "gl/shader.hpp"
#include "gfx/rectangle.hpp"

using GameObjects::StaticRectangle;

StaticRectangle::StaticRectangle(Rect<float> rect, glm::vec4 const& color)
    : rect(rect), color(color)
{}

void StaticRectangle::Draw()
{
    Gfx::Rectangle::Draw(rect, color);
}

