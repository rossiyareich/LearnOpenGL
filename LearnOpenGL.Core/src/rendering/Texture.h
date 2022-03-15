#pragma once
#include <cstdint>
#include <functional>

#include <glew/glew.h>

namespace rendering
{
    class Texture
    {
    public:
        uint32_t TextureID;
        GLenum TextureTarget;
        Texture(const char* file, GLenum target, bool flipped = false);
        Texture(const char* const file, const GLenum target, const bool flipped,
                std::function<void(const GLenum target)> postAction);
        ~Texture();
        void Bind() const;
        void Unbind() const;
        static void Activate(GLenum target);
    };
}
