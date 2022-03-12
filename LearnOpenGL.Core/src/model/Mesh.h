#pragma once
#include <vector>

#include "TextureData.h"
#include "Vertex.h"

#include "../rendering/Buffer.h"
#include "../rendering/VertexArray.h"

namespace model
{
    class Mesh
    {
    public:
        std::vector<model::Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<model::TextureData> TextureData;
        Mesh(const std::vector<model::Vertex>& vertices,
             const std::vector<uint32_t>& indices,
             const std::vector<model::TextureData>& textureData);
    private:
        rendering::Buffer &vbo, &ebo;
        rendering::VertexArray& vao;
        void SetupMesh();
    };
}
