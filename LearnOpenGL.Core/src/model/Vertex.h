#pragma once
#include <glm/vec3.hpp>

namespace model
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 TexCoords;
    };
}
