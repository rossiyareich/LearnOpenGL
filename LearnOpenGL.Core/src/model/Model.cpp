#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../logging/ConsoleLogger.h"

namespace model
{
    Model::Model(const char* filePath) : filePath(filePath)
    {
        parentDirectory = this->filePath.substr(0, this->filePath.find_last_of('/')) + '/';

        Assimp::Importer importer{};
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            logging::ConsoleLogger::Get().SetLogLevel(LogLevel::Error);
            logging::ConsoleLogger::Get().WriteLine(
                "Assimp error: LoadModel: " + std::string(importer.GetErrorString()));
            logging::ConsoleLogger::Get().SetLogLevel(LogLevel::Debug);
            assert(false);
        }

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::Draw(const rendering::ShaderProgram& program) const
    {
        for (auto& mesh : meshes)
            mesh.Draw(program);
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // Process all meshes of the node
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* currentMesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(currentMesh, scene));
        }

        // Invoke all child nodes
        for (size_t i = 0; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene);
    }

    Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<TextureData> textureData;

        // Retrieve all vertex data
        for (size_t i = 0; i < mesh->mNumVertices; i++)
        {
            // Retrieve all texCoords
            glm::vec2 texCoords{};
            if (mesh->mTextureCoords[0])
            {
                texCoords.x = mesh->mTextureCoords[0][i].x;
                texCoords.y = mesh->mTextureCoords[0][i].y;
            }

            Vertex vertex{
                glm::vec3{
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                },
                glm::vec3{
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                },
                texCoords
            };
            vertices.push_back(vertex);
        }

        // Retrieve all indices
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            {
                indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }

        // Retrieve material data
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            auto diffuse{LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse")};
            textureData.insert(textureData.end(), diffuse.begin(), diffuse.end());

            auto specular{LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular")};
            textureData.insert(textureData.end(), specular.begin(), specular.end());

            auto emission{LoadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emission")};
            textureData.insert(textureData.end(), emission.begin(), emission.end());
        }

        return {vertices, indices, textureData};
    }

    std::vector<TextureData> Model::LoadMaterialTextures(aiMaterial* material,
                                                         aiTextureType type,
                                                         const std::string& typeName)
    {
        std::vector<TextureData> textureData{};

        for (size_t i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString path{}; // File that contains the actual data
            material->GetTexture(type, i, &path);
            std::string pathStr = path.C_Str();

            bool isCached{};
            for (const auto& cached : textureCache)
            {
                // Match
                if (cached.RelativePath == pathStr)
                {
                    textureData.push_back(cached);
                    isCached = true;
                    break;
                }
            }

            if (!isCached)
            {
                TextureData texture
                {
                    rendering::Texture{
                        (parentDirectory + std::string(path.C_Str())).c_str(),
                        GL_TEXTURE_2D,
                        true,
                        [](const GLenum target)
                        {
                            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        }
                    },
                    typeName,
                    pathStr
                };

                textureData.push_back(texture);
                textureCache.push_back(texture);
            }
        }

        return textureData;
    }
}
