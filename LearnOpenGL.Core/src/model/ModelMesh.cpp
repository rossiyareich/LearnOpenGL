#include "ModelMesh.h"
#include "../helpers/Constants.h"

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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),
                              reinterpret_cast<void*>(offsetof(ModelVertex, Normal)));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),
                              reinterpret_cast<void*>(offsetof(ModelVertex, TexCoords)));
        vao.Unbind();
    }

    void ModelMesh::Draw(const rendering::ShaderProgram& shader) const
    {
        uint32_t diffuseN{1};
        uint32_t specularN{1};
        uint32_t emissionN{1};

        vao.Bind();

        for (uint32_t i = 0; i < textures.size(); i++)
        {
            rendering::Texture::Activate(GL_TEXTURE0 + i);
            auto& texture = textures[i];
            texture.Bind();

            std::string currentN{};
            switch (texture.Type)
            {
            case ModelTextureType::texture_diffuse:
                currentN = std::to_string(diffuseN++);
                break;
            case ModelTextureType::texture_emission:
                currentN = std::to_string(emissionN++);
                break;
            case ModelTextureType::texture_specular:
                currentN = std::to_string(specularN++);
                break;
            default:
                assert(false);
            }

            shader.SetUFInt((ModelTextureTypeToString(texture.Type) + currentN).c_str(), i);
        }

        rendering::Texture::Activate(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(indices.size()), GL_UNSIGNED_INT, nullptr);

        vao.Unbind();
    }
}
