#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace model
{
    struct Vertex
    {
        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords) :
            Position(position), Normal(normal), TexCoords(texCoords)
        {
        }

        glm::vec3 Position{};
        glm::vec3 Normal{};
        glm::vec2 TexCoords{};
    };
}
