#pragma once
#include <string>

#include "../rendering/Texture.h"

namespace model
{
    struct TextureData
    {
        TextureData();

        TextureData(rendering::Texture& texture, const std::string& representationType,
                    const std::string& relativePath) : Texture(texture), RepresentationType(representationType),
                                                       RelativePath(relativePath)
        {
        }

        TextureData(rendering::Texture&& texture, const std::string& representationType,
                    const std::string& relativePath) : Texture(texture), RepresentationType(representationType),
                                                       RelativePath(relativePath)
        {
        }

        TextureData(const TextureData& other) : Texture(other.Texture), RepresentationType(other.RepresentationType),
                                                RelativePath(other.RelativePath)
        {
        }

        TextureData& operator=(const TextureData& other)
        {
            Texture = other.Texture;
            RepresentationType = other.RepresentationType;
            RelativePath = other.RelativePath;
            return *this;
        }

        rendering::Texture& Texture;
        std::string RepresentationType; // Diffuse, Specular, etc.
        std::string RelativePath;
    };
}
