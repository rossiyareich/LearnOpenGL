#pragma once
#include <memory>

#include "Mesh.h"

#include "../rendering/Shader.h"
#include "../rendering/ShaderProgram.h"
#include <assimp/scene.h>

namespace model
{
    class Model
    {
    public:
        Model(const char* filePath);
        void Draw(const rendering::ShaderProgram& program) const;
    private:
        std::vector<Mesh> meshes{};
        std::vector<TextureData> textureCache{};
        std::string parentDirectory{}; // For loading textures contained within the same directory
        std::string filePath{}; // The mesh file path itself

        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<TextureData> LoadMaterialTextures(aiMaterial* material,
                                                      aiTextureType type,
                                                      const std::string& typeName);
    };
}
