#pragma once
#include "ModelTextureType.h"
#include "../rendering/Texture.h"

namespace model
{
    struct ModelTexture : public rendering::Texture
    {
        ModelTextureType Type;

        ModelTexture(const char* file, GLenum target, ModelTextureType type, bool flipped = false) :
            Texture(file, target, flipped), Type(type) {}
    };
}
