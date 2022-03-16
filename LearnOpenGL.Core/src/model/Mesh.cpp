#include "Mesh.h"

#include "../rendering/ShaderProgram.h"

namespace model
{
    Mesh::Mesh(std::vector<Vertex> vertices,
               std::vector<uint32_t> indices,
               std::vector<model::TextureData> textureData) :
        Vertices(std::move(vertices)),
        Indices(std::move(indices)),
        TextureData(std::move(textureData))
    {
        vbo.Bind();
        vbo.PushArray<const std::vector<model::Vertex>&>(Vertices, GL_STATIC_DRAW);
        vbo.Unbind();

        ebo.Bind();
        ebo.PushArray<const std::vector<uint32_t>&>(Indices, GL_STATIC_DRAW);
        ebo.Unbind();

        vao.Bind();

        vbo.Bind();
        ebo.Bind();

        /* Set vbo vertex attributes */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(0); // Vertex positions @ location = 0
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, Normal)));
        glEnableVertexAttribArray(1); // Vertex normals @ location = 1
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
        glEnableVertexAttribArray(2); // TexCoords @ location = 2

        vao.Unbind();
    }

    /**
     * \brief Used with textures with no ambient
     */
    void Mesh::Draw(const rendering::ShaderProgram& program) const
    {
        uint32_t diffuseNr{};
        uint32_t specularNr{};
        uint32_t emissionNr{};

        vao.Bind();

        for (size_t i{}; i < TextureData.size(); i++)
        {
            auto& texture = TextureData[i].Texture;
            auto& representationType = TextureData[i].RepresentationType;

            std::string totalTexturesPrefix{};

            rendering::Texture::Activate(GL_TEXTURE0 + i);
            texture.Bind();

            if (representationType == "texture_diffuse")
                totalTexturesPrefix = std::to_string(diffuseNr++);
            else if (representationType == "texture_specular")
                totalTexturesPrefix = std::to_string(specularNr++);
            else if (representationType == "texture_emission")
                totalTexturesPrefix = std::to_string(emissionNr++);

            program.SetUFInt((representationType + "[" + totalTexturesPrefix + "]").c_str(), i);
        }

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        rendering::Texture::Activate(GL_TEXTURE0);

        vao.Unbind();
    }
}
