#pragma once
#include <vector>

#include "Shader.h"

namespace rendering
{
    class ShaderProgram
    {
    public:
        uint32_t ProgramID;
        ShaderProgram(Shader& vertex, Shader& fragment);
        ~ShaderProgram();
        void Use() const;
        void Unuse() const;

        void SetUFBool(const char* name, bool value) const;
        void SetUFInt(const char* name, int value) const;
        void SetUFFloat(const char* name, float value) const;
    };
}
