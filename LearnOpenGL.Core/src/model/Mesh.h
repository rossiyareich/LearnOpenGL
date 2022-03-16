#pragma once
#include <vector>

#include "TextureData.h"
#include "Vertex.h"

#include "../rendering/Buffer.h"
#include "../rendering/VertexArray.h"
#include "../rendering/ShaderProgram.h"

namespace model
{
    class Mesh
    {
    public:
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<TextureData> TextureData;
        Mesh(std::vector<Vertex> vertices,
             std::vector<uint32_t> indices,
             std::vector<model::TextureData> textureData);
        void Draw(const rendering::ShaderProgram& program) const;
    private:
        const rendering::VertexArray& vao{};
        const rendering::Buffer& vbo{GL_ARRAY_BUFFER};
        const rendering::Buffer& ebo{GL_ELEMENT_ARRAY_BUFFER};
    };
}
