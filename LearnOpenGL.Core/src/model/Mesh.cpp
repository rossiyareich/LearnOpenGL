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
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);


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

        glBindVertexArray(0);
    }

    /**
     * \brief Used with textures with no ambient
     */
    void Mesh::Draw(const rendering::ShaderProgram& program) const
    {
        uint32_t diffuseNr{};
        uint32_t specularNr{};
        uint32_t emissionNr{};

        for (size_t i{}; i < TextureData.size(); i++)
        {
            auto& texture = TextureData[i].Texture;
            auto& representationType = TextureData[i].RepresentationType;

            std::string totalTexturesPrefix{};

            rendering::Texture::Activate(GL_TEXTURE0 + i);
            texture.Bind();

            if (representationType == "texture_diffuse")
                totalTexturesPrefix = std::to_string(diffuseNr++);

            program.SetUFInt((representationType + totalTexturesPrefix).c_str(), i);
        }

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        rendering::Texture::Activate(GL_TEXTURE0);
    }
}
