#include "Model.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../logging/ConsoleLogger.h"

namespace model
{
    Model::Model(const std::string& modelPath)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            logging::ConsoleLogger::Get().WriteLine("ERROR::ASSIMP::" + std::string(import.GetErrorString()) + "\n",
                                                    LogLevel::Error);
            assert(false);
            return;
        }

        parentDirectory = modelPath.substr(0, modelPath.find_last_of('/')) + "/"s;
        ProcessNode(scene->mRootNode, scene);
    }

    void Model::Draw(const rendering::ShaderProgram& shader)
    {
        for (const auto& mesh : meshes)
            mesh.Draw(shader);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    ModelMesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<ModelVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<ModelTexture> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            // process vertex positions, normals and texture coordinates
            ModelVertex vertex
            {
                {
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                },
                {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                },
                {}
            };

            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }

            vertices.push_back(vertex);
        }

        // process indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process material
        // ReSharper disable once CppUnsignedZeroComparison
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            LoadMaterialTextures(textures, material,
                aiTextureType_DIFFUSE, ModelTextureType::texture_diffuse);
            LoadMaterialTextures(textures, material,
                aiTextureType_SPECULAR, ModelTextureType::texture_specular);
            LoadMaterialTextures(textures, material,
                aiTextureType_EMISSIVE, ModelTextureType::texture_emission);
        }

        ModelMesh retMesh{vertices, indices, textures};

        return retMesh;
    }

    void Model::LoadMaterialTextures(std::vector<ModelTexture>& textures, aiMaterial* mat, aiTextureType type,
                                                          ModelTextureType textureType)
    {
        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string fullPath(parentDirectory + std::string(str.C_Str()));

            std::unordered_map<std::string, ModelTexture>::const_iterator iter;
            if ((iter = loadedTextures.find(fullPath)) == loadedTextures.end())
            {
                ModelTexture texture
                {
                    fullPath.c_str(),
                    GL_TEXTURE_2D,
                    textureType,
                    true
                };

                textures.push_back(texture);
                loadedTextures[fullPath] = texture;
            }
            else
            {
                textures.push_back(iter->second);
            }
        }
    }
}
