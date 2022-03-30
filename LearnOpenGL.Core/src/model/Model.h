#pragma once

#include <unordered_map>

#include "ModelMesh.h"
#include "../rendering/ShaderProgram.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>

namespace model
{
    class Model
    {
    public:
        Model(const std::string& modelPath);
        void Draw(const rendering::ShaderProgram& shader);
    private:
        // model data
        std::vector<ModelMesh> meshes;
        std::string parentDirectory;
        std::unordered_map<std::string, ModelTexture> loadedTextures;

        void ProcessNode(aiNode* node, const aiScene* scene);
        [[nodiscard]] ModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void LoadMaterialTextures
        (std::vector<ModelTexture>& textures, aiMaterial* mat, aiTextureType type, ModelTextureType textureType);
    };
}
