#pragma once
#include <vector>

#include "Shader.h"

namespace rendering
{
    class ShaderProgram
    {
    public:
        uint32_t ProgramID;
        ShaderProgram();
        ShaderProgram(Shader&& vertex, Shader&& fragment);
        ShaderProgram(Shader& vertex, Shader& fragment);
        ~ShaderProgram();
        void Use() const;
        void Unuse() const;

        void SetUFBool(const char* name, bool value) const;
        void SetUFInt(const char* name, int value) const;
        void SetUFFloat(const char* name, float value) const;
        void SetUFMatrix4Float(const char* name, const GLfloat* value, GLboolean transpose = GL_FALSE) const;
        void SetUFVector3(const char* name, float x, float y, float z) const;
        void SetUFUint32(const char* name, uint32_t value) const;
    };
}
