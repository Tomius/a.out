#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include "gl/shader.hpp"
#include "gl/buffer.hpp"

namespace Gfx
{

class Pixel {
public:
    template <typename Material>
    static void Draw(glm::vec2 coord, const Material& material,
                     const glm::mat3& mvp = glm::mat3{}) {
        PixelImpl<Material>::Draw(coord, material, mvp);
    }

private:
    template<typename Material>
    class PixelImpl {
    public:
        static void Draw(glm::vec2 coord, const Material& material,
                         const glm::mat3& mvp);

    private:
        Gl::Program program;
        Gl::VertexArray vao;

        gl33::GLint uloc_pos;
        gl33::GLint uloc_color;
        gl33::GLint uloc_mvp;

        PixelImpl();

        static PixelImpl& GetInstance();
    };

};

}

#include "pixel_impl.hpp"
