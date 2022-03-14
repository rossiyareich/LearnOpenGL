#pragma once
#include <cstdint>
#include <vector>

#include <glew/glew.h>
#include "../model/Vertex.h"

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
            // Push vertex to buffer
            glBufferData(BufferTarget, sizeof(arr), &arr, usage);
        }

        template <const std::vector<model::Vertex>&>
        void PushArray(const std::vector<model::Vertex>& vertices, GLenum usage)
        {
            // Push vertices to buffer
            glBufferData(BufferTarget, vertices.size() * sizeof(model::Vertex), &vertices[0], usage);
        }

        template <const std::vector<uint32_t>&>
        void PushArray(const std::vector<uint32_t>& indices, GLenum usage)
        {
            // Push indices to buffer
            glBufferData(BufferTarget, indices.size() * sizeof(uint32_t), &indices[0], usage);
        }
    };
}
