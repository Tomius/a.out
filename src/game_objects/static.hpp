#ifndef UUID_407F9AC5_BF7C_4D03_A71E_DBB039E16F76
#define UUID_407F9AC5_BF7C_4D03_A71E_DBB039E16F76
#pragma once

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "game_objects/game_object.hpp"

namespace GameObjects
{

class Static : public GameObject
{
public:
    Static(Rect<float> rect);

    void Draw();

private:
    void SetVbo();

    Rect<float> rect;
    Gl::Program prog; // todo: shader cache...
    Gl::VertexArray vao;
    Gl::Buffer buf;
};

}

#endif
