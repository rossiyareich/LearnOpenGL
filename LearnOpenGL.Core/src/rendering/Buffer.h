#pragma once
#include <cstdint>
#include <vector>

#include <glew/glew.h>
#include "../model/ModelVertex.h"

namespace rendering
{
    class Buffer
    {
    public:
        uint32_t BufferID;
        GLenum BufferTarget;

        Buffer(GLenum target);
        ~Buffer();

        void Bind() const;
        void Unbind() const;

        template <typename T>
        void PushArray(T& arr, GLenum usage) const
        {
            glBufferData(BufferTarget, sizeof(arr), arr, usage); // Push vertex to buffer
        }

        template <>
        void PushArray<const std::vector<model::ModelVertex>>(const std::vector<model::ModelVertex>& vertices,
                                                              GLenum usage) const
        {
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(model::ModelVertex), &vertices[0], usage);
        }

        template <>
        void PushArray<const std::vector<uint32_t>>(const std::vector<uint32_t>& indices, GLenum usage) const
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
        }
    };
}
