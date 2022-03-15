#include "ShaderProgram.h"

#include <cassert>
#include <iostream>

#include "../helpers/Constants.h"

#include "../logging/ConsoleLogger.h"

namespace rendering
{
    ShaderProgram::ShaderProgram() : ProgramID{}
    {
    }

    ShaderProgram::ShaderProgram(Shader&& vertex, Shader&& fragment) : ProgramID{glCreateProgram()}
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
            logging::ConsoleLogger::Get().SetLogLevel(LogLevel::Error);
            logging::ConsoleLogger::Get().WriteLine(infoLog);
            logging::ConsoleLogger::Get().SetLogLevel(LogLevel::Debug);

            assert(false);
        }
    }

    ShaderProgram::ShaderProgram(Shader& vertex, Shader& fragment) : ShaderProgram(
        std::move(vertex), std::move(fragment))
    {
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

    void ShaderProgram::SetUFVector3(const char* name, const float x, const float y, const float z) const
    {
        glUniform3f(glGetUniformLocation(ProgramID, name), x, y, z);
    }

    void ShaderProgram::SetUFUint32(const char* name, const uint32_t value) const
    {
        glUniform1ui(glGetUniformLocation(ProgramID, name), value);
    }

    ShaderProgram::~ShaderProgram()
    {
        Unuse();
        glDeleteProgram(ProgramID);
    }
}
