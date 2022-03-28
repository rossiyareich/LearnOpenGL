#include "ModelMesh.h"

namespace model
{
    ModelMesh::ModelMesh(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices,
        const std::vector<ModelTexture>& textures) :
        vertices(vertices),
        indices(indices),
        textures(textures),
        vao(),
        vbo(GL_ARRAY_BUFFER),
        ebo(GL_ELEMENT_ARRAY_BUFFER)

    {
        vbo.Bind();
        vbo.PushArray(vertices, GL_STATIC_DRAW);
        vbo.Unbind();

        ebo.Bind();
        ebo.PushArray(indices, GL_STATIC_DRAW);
        ebo.Unbind();

        vao.Bind();
        vbo.Bind();
        ebo.Bind();

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), reinterpret_cast<void*>(0));
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), reinterpret_cast<void*>(offsetof(ModelVertex, Normal)));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), reinterpret_cast<void*>(offsetof(ModelVertex, TexCoords)));
        vao.Unbind();
    }

    void ModelMesh::Draw(const rendering::ShaderProgram& shader)
    {
    }
}
