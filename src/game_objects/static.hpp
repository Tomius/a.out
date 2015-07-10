#pragma once

#include "game_objects/game_object.hpp"
#include "video/camera.hpp"

#include "gfx/circle.hpp"
#include "gfx/line.hpp"
#include "gfx/pixel.hpp"
#include "gfx/rectangle.hpp"

namespace GameObjects
{

template<typename T>
class Static : public GameObject
{
public:
    template<typename... Args>
    Static(Args&&... args) : draw_func([=]()
    {
        T::Draw(args..., GetScene().GetCamera().GetMatrix());
    }) {}

    // ezzel valamit kurvara kezdeni kene
    Static(Rect<float> rect, Gl::Texture&& tex, glm::vec4 color)
    {
        auto ptex = std::make_shared<Gl::Texture>(std::move(tex));
        draw_func = [=]()
        {
            T::Draw(rect, *ptex, color, GetScene().GetCamera().GetMatrix());
        };
    }

    void Draw() override {
        draw_func();
    }

private:
    std::function<void()> draw_func;
};

using StaticCircle = GameObjects::Static<Gfx::Circle>;
using StaticLine = GameObjects::Static<Gfx::Line>;
using StaticPixel = GameObjects::Static<Gfx::Pixel>;
using StaticRectangle = GameObjects::Static<Gfx::Rectangle>;

}
