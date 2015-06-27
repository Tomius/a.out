#include "game_objects/static.hpp"
#include "gl/shader.hpp"

namespace GameObjects
{

Static::Static(Rect<float> rect)
    : rect(rect),
      prog{Gl::Shader{gl33::GL_VERTEX_SHADER,
                      "#version 330\n"
                      "in vec2 position;\n"
                      "void main()\n"
                      "{\n"
                      "    gl_Position = vec4(position, 0, 1);\n"
                      "}\n"
                     },
           Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                      "#version 330\n"
                      "out vec4 col;\n"
                      "void main()\n"
                      "{\n"
                      "    col = vec4(1, 1, 1, 1);\n"
                      "}"
                     }}
{
    SetVbo();
    vao.Bind();
    auto pos = prog.GetAttribLocation("position");
    gl33::glEnableVertexAttribArray(pos);
    gl33::glVertexAttribPointer(pos, 2, gl33::GL_FLOAT, gl33::GL_FALSE, 0, 0);

    prog.BindFragDataLocation(0, "col");
}

void Static::Draw()
{
    prog.Use();
    vao.Bind();
    buf.Bind(gl33::GL_ARRAY_BUFFER);
    gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
}

void Static::SetVbo()
{
    float cbuf[] = {
        rect.x,            rect.y,
        rect.x,            rect.y+rect.height,
        rect.x+rect.width, rect.y+rect.height,
        rect.x+rect.width, rect.y,
    };
    buf.Bind(gl33::GL_ARRAY_BUFFER);
    gl33::glBufferData(gl33::GL_ARRAY_BUFFER, sizeof(cbuf), cbuf,
                       gl33::GL_STATIC_DRAW);
}

}
