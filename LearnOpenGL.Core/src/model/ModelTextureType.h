#pragma once
#include <cassert>
#include <string>

using std::string_literals::operator ""s;

namespace model
{
    enum class ModelTextureType
    {
        texture_diffuse,
        texture_specular,
        texture_emission
    };

    [[nodiscard]] constexpr std::string ModelTextureTypeToString(const ModelTextureType type)
    {
        switch (type)
        {
        case ModelTextureType::texture_diffuse:
            return "texture_diffuse"s;
        case ModelTextureType::texture_emission:
            return "texture_emission"s;
        case ModelTextureType::texture_specular:
            return "texture_specular"s;
        }

        assert(false);
        return nullptr;
    }
}
