#pragma once

#include "gl/shader.hpp"

namespace Gfx {

class ColorMaterial {
public:
    ColorMaterial(glm::vec4 color) : color(color) {}

    static const Gl::Shader& GetShader() {
        static Gl::Shader shader{gl33::GL_FRAGMENT_SHADER,
            "#version 330\n"
            "uniform vec4 ColorMaterial_color;\n"
            "vec4 GetColor(vec2 texCoord)\n"
            "{\n"
            "    return ColorMaterial_color;\n"
            "}\n"};
        return shader;
    }

    void Update(const Gl::Program& program) const {
        gl33::GLint uloc_color = program.GetUniformLocation("ColorMaterial_color");
        ASSERT(uloc_color != -1);
        gl33::glUniform4fv(uloc_color, 1, glm::value_ptr(color));
    }

private:
    glm::vec4 color;
};

}
