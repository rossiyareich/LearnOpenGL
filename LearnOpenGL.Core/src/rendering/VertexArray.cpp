#include "VertexArray.h"

#include <glew/glew.h>

namespace rendering
{
    VertexArray::VertexArray() : ArrayID{}
    {
        glGenVertexArrays(1, &ArrayID);
    }

    //VertexArray::~VertexArray()
    //{
    //    Unbind();
    //    glDeleteVertexArrays(1, &ArrayID);
    //}

    void VertexArray::Bind() const
    {
        glBindVertexArray(ArrayID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}
