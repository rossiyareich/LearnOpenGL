#include "Mesh.h"

#include "../rendering/ShaderProgram.h"

namespace model
{
    Mesh::Mesh(const std::vector<Vertex>& vertices,
               const std::vector<uint32_t>& indices,
               const std::vector<model::TextureData>& textureData) :
        Vertices(vertices),
        Indices(indices),
        TextureData(textureData),
        vao(),
        vbo(GL_ARRAY_BUFFER),
        ebo(GL_ELEMENT_ARRAY_BUFFER)
    {
        SetupMesh();
    }

    void Mesh::SetupMesh()
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
        glEnableVertexAttribArray(0); // Vertex positions @ location = 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(1); // Vertex normals @ location = 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, Normal)));
        glEnableVertexAttribArray(2); // TexCoords @ location = 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

        vao.Unbind();
    }

    /**
     * \brief Used with textures with no ambient
     */
    void Mesh::Draw(const rendering::ShaderProgram& program)
    {
        uint32_t diffuseNr{};
        uint32_t specularNr{};
        uint32_t emissionNr{};

        for (size_t i{}; i < TextureData.size(); i++)
        {
            auto& texture = TextureData[i].Texture;
            auto& representationType = TextureData[i].RepresentationType;

            std::string totalTexturesPrefix{};

            texture.Bind();
            rendering::Texture::Activate(GL_TEXTURE0 + i);

            if (representationType == "texture_diffuse")
                totalTexturesPrefix = std::to_string(diffuseNr++);
            else if (representationType == "texture_specular")
                totalTexturesPrefix = std::to_string(specularNr++);
            else if (representationType == "texture_emission")
                totalTexturesPrefix = std::to_string(emissionNr++);

            program.SetUFInt((representationType + "[" + totalTexturesPrefix + "]").c_str(), i);
        }

        vao.Bind();
        vbo.Bind();
        ebo.Bind();

        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
        vao.Unbind();
        rendering::Texture::Activate(GL_TEXTURE0);
    }
}
