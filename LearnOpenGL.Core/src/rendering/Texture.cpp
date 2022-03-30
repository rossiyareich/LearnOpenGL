#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <glew/glew.h>

#include <stb/stb_image.h>

namespace rendering
{
    Texture::Texture()
    {
    }

    Texture::Texture(const char* const file, const GLenum target, const bool flipped) : TextureID{},
        TextureTarget{target}
    {
        int width, height, colorChannels;
        stbi_set_flip_vertically_on_load(flipped);
        auto* data = stbi_load(file, &width, &height, &colorChannels, 0);
        assert(data);

        glGenTextures(1, &TextureID);
        Bind();

        GLenum format{};
        if (colorChannels == 3)
            format = GL_RGB;
        else if (colorChannels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB;

        switch (target)
        {
        case GL_TEXTURE_2D:
            glTexImage2D(TextureTarget, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            break;
        default:
            assert(false);
        }

        glGenerateMipmap(TextureTarget);
        stbi_image_free(data);
        Unbind();
    }

    //Texture::~Texture()
    //{
    //    glDeleteTextures(1, &TextureID);
    //}

    void Texture::Bind() const
    {
        glBindTexture(TextureTarget, TextureID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(TextureTarget, 0);
    }

    void Texture::Activate(GLenum target)
    {
        glActiveTexture(target);
    }
}
