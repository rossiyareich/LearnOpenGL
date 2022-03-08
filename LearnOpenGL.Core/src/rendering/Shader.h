#pragma once
#include <cstdint>
#include <glew/glew.h>

namespace rendering
{
    class Shader
    {
    public:
        uint32_t ShaderID;

        Shader(const char* path, GLenum type);
        ~Shader();
    };
}
