#pragma once
#include <string>

#include "../rendering/Texture.h"

namespace model
{
    struct TextureData
    {
        rendering::Texture& Texture;
        const std::string& RepresentationType; // Diffuse, Specular, etc.
    };
}
