#pragma once
#include <vector>

#include "ModelTexture.h"
#include "ModelVertex.h"
#include "../rendering/Shader.h"
#include "../rendering/VertexArray.h"
#include "../rendering/Buffer.h"
#include "../rendering/ShaderProgram.h"

namespace model
{
    class ModelMesh
    {
    private:
        std::vector<ModelVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<ModelTexture> textures;
    private:
        rendering::VertexArray vao;
        rendering::Buffer vbo;
        rendering::Buffer ebo;
    public:
        ModelMesh(const std::vector<ModelVertex>& vertices,
                  const std::vector<uint32_t>& indices,
                  const std::vector<ModelTexture>& textures);
        void Draw(const rendering::ShaderProgram& shader);

    };
}
