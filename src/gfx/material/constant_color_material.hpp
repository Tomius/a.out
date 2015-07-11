#pragma once

#include "gl/shader.hpp"

namespace Gfx {

class ConstantColorMaterial {
public:
    ConstantColorMaterial(glm::vec4 color) : color(color) {}

    static const Gl::Shader& GetShader() {
        static Gl::Shader shader{gl33::GL_FRAGMENT_SHADER,
            "#version 330\n"
            "uniform vec4 ConstantColorMaterial_color;\n"
            "vec4 GetColor(vec2 texCoord)\n"
            "{\n"
            "    return ConstantColorMaterial_color;\n"
            "}\n"};
        return shader;
    }

    void UploadUniforms(const Gl::Program& program) const {
        gl33::GLint uloc_color = program.GetUniformLocation("ConstantColorMaterial_color");
        ASSERT(uloc_color != -1);
        gl33::glUniform4fv(uloc_color, 1, glm::value_ptr(color));
    }

private:
    glm::vec4 color;
};

}
