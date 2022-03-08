#pragma once
#include <cstdint>

namespace rendering
{
    class VertexArray
    {
    public:
        uint32_t ArrayID;

        VertexArray();
        ~VertexArray();
        void Bind() const;
        void Unbind() const;
    };
}
