#pragma once
#include <cstdint>

#include <glew/glew.h>

namespace rendering
{
    class Texture
    {
    public:
        uint32_t TextureID;
        GLenum TextureTarget;
        Texture(const char* file, GLenum target, bool flipped = false);
        ~Texture();
        void Bind() const;
        void Unbind() const;
        static void Activate(GLenum target);
    };
}
