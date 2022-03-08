#include "ShaderProgram.h"

#include <cassert>
#include <iostream>

#include "../helpers/Constants.h"

namespace rendering
{
    ShaderProgram::ShaderProgram(Shader& vertex, Shader& fragment) : ProgramID{glCreateProgram()}
    {
        glAttachShader(ProgramID, vertex.ShaderID);
        glAttachShader(ProgramID, fragment.ShaderID);

        glLinkProgram(ProgramID);
        int success;
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glErrorLog infoLog;
            glGetProgramInfoLog(ProgramID, GL_ERROR_LENGTH, nullptr, infoLog);
            std::cout << infoLog << std::endl;
            assert(false);
        }

        vertex.~Shader();
        fragment.~Shader();
    }

    void ShaderProgram::Use() const
    {
        glUseProgram(ProgramID);
    }

    void ShaderProgram::Unuse() const
    {
        glUseProgram(0);
    }

    void ShaderProgram::SetUFBool(const char* const name, const bool value) const
    {
        glUniform1i(glGetUniformLocation(ProgramID, name), static_cast<int>(value));
    }

    void ShaderProgram::SetUFInt(const char* const name, const int value) const
    {
        glUniform1i(glGetUniformLocation(ProgramID, name), value);
    }

    void ShaderProgram::SetUFFloat(const char* const name, const float value) const
    {
        glUniform1f(glGetUniformLocation(ProgramID, name), value);
    }

    void ShaderProgram::SetUFMatrix4Float(const char* name, const GLfloat* const value,
                                          GLboolean transpose) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ProgramID, name), 1, transpose, value);
    }


    ShaderProgram::~ShaderProgram()
    {
        Unuse();
        glDeleteProgram(ProgramID);
    }
}
