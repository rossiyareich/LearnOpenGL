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
        uint32_t Shininess;
        Mesh(const std::vector<Vertex>& vertices,
             const std::vector<uint32_t>& indices,
             const std::vector<model::TextureData>& textureData);
        void Draw(const rendering::ShaderProgram& program);
    private:
        rendering::Buffer vbo, ebo;
        rendering::VertexArray vao;
        void SetupMesh();
    };
}
