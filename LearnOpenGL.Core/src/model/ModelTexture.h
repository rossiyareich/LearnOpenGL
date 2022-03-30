#pragma once
#include "ModelTextureType.h"
#include "../rendering/Texture.h"

namespace model
{
    struct ModelTexture : public rendering::Texture
    {
        ModelTextureType Type;

        ModelTexture() : Texture()
        {
        }

        ModelTexture(const ModelTexture& other) noexcept = default;
        ModelTexture& operator=(const ModelTexture& other) noexcept = default;
        ModelTexture(ModelTexture&& other) noexcept = default;
        ModelTexture& operator=(ModelTexture&& other) noexcept = default;

        ModelTexture(const char* file, GLenum target, ModelTextureType type, bool flipped = false) :
            Texture(file, target, flipped), Type(type)
        {
        }
    };
}
