#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/rectangle.hpp"
#include "gl/shader.hpp"
#include "misc/exception.hpp"

using Gfx::Rectangle;

Rectangle::Rectangle()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330 core\n"
                  "const vec2 data[4] = vec2[](\n"
                  "    vec2(0, 0),\n"
                  "    vec2(0, 1),\n"
                  "    vec2(1, 1),\n"
                  "    vec2(1, 0)\n"
                  ");\n"
                  "const vec2 coord[4] = vec2[](\n"
                  "    vec2(0, 1),\n"
                  "    vec2(0, 0),\n"
                  "    vec2(1, 0),\n"
                  "    vec2(1, 1)\n"
                  ");\n"
                  "uniform vec2 bottom_left;\n"
                  "uniform vec2 size;\n"
                  "uniform mat3 mvp;\n"
                  "out vec2 texcoord;\n"
                  "void main()\n"
                  "{\n"
                  "    vec2 pos = bottom_left + size*data[gl_VertexID];\n"
                  "    texcoord = coord[gl_VertexID];\n"
                  "    gl_Position = vec4((mvp*vec3(pos,1)).xy, 0, 1);\n"
                  "}\n"
              },
              Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                  "#version 330 core\n"
                  "in vec2 texcoord;\n"
                  "out vec4 col;\n"
                  "uniform vec4 color;\n"
                  "uniform sampler2D tex;\n"
                  "void main()\n"
                  "{\n"
                  "    col = color * texture2D(tex, texcoord);\n"
                  "}"
              }}
{
    program.Use();
    program.BindFragDataLocation(0, "col");
    uloc_bottom_left = program.GetUniformLocation("bottom_left");
    uloc_size = program.GetUniformLocation("size");
    uloc_color = program.GetUniformLocation("color");
    uloc_mvp = program.GetUniformLocation("mvp");
    uloc_tex = program.GetUniformLocation("tex");

    ASSERT(uloc_bottom_left != -1);
    ASSERT(uloc_size != -1);
    ASSERT(uloc_color != -1);
    ASSERT(uloc_mvp != -1);
    ASSERT(uloc_tex != -1);

    tex.Bind();
    uint8_t buf[4] = { 255, 255, 255, 255 };
    gl33::glTexImage2D(gl33::GL_TEXTURE_2D, 0,
                       static_cast<gl33::GLint>(gl33::GL_RGB8), 1, 1, 0,
                       gl33::GL_RGB, gl33::GL_UNSIGNED_BYTE, buf);
    gl33::glUniform1i(uloc_tex, 0);
}

void Rectangle::Draw(Rect<float> rect, Gl::Texture& tex, glm::vec4 const& color,
                     glm::mat3 const& mvp)
{
    GetInstance().program.Use();
    GetInstance().vao.Bind();
    tex.Bind();
    gl33::glUniform2fv(GetInstance().uloc_bottom_left, 1, &rect.x);
    gl33::glUniform2fv(GetInstance().uloc_size, 1, &rect.width);
    gl33::glUniform4fv(GetInstance().uloc_color, 1, glm::value_ptr(color));
    gl33::glUniformMatrix3fv(GetInstance().uloc_mvp, 1,
                             gl33::GL_FALSE, glm::value_ptr(mvp));
    gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
}

void Rectangle::Draw(Rect<float> rect, const glm::vec4& color,
                     const glm::mat3& mvp)
{
    Draw(rect, GetInstance().tex, color, mvp);
}

Rectangle& Rectangle::GetInstance()
{
    static Rectangle rectangle;
    return rectangle;
}
