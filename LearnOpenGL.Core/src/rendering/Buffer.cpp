#include "Buffer.h"

#include <glew/glew.h>

#include <array>

namespace rendering
{
    Buffer::Buffer(GLenum target) : BufferID{}, BufferTarget{target}
    {
        glGenBuffers(1, &BufferID);
    }

    Buffer::~Buffer()
    {
        Unbind();
        glDeleteBuffers(1, &BufferID);
    }

    void Buffer::Bind() const
    {
        glBindBuffer(BufferTarget, BufferID);
    }

    void Buffer::Unbind() const
    {
        glBindBuffer(BufferTarget, 0);
    }
}
