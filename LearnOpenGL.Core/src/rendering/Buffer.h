#pragma once
#include <cstdint>

#include <glew/glew.h>

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
    };
}
