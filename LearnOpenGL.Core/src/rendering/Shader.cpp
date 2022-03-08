#include "Shader.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include <glew/glew.h>

#include "../helpers/Constants.h"

namespace rendering
{
    Shader::Shader(const char* const path, const GLenum type) : ShaderID{glCreateShader(type)}
    {
        std::ifstream file{};
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        file.open(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        const std::string codeStr = buffer.str();
        const char* code = codeStr.c_str();

        glShaderSource(ShaderID, 1, &code, nullptr);

        glCompileShader(ShaderID);
        int success;
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glErrorLog infoLog;
            glGetShaderInfoLog(ShaderID, GL_ERROR_LENGTH, nullptr, infoLog);
            std::cout << infoLog << std::endl;
            assert(false);
        }
    }

    Shader::~Shader()
    {
        glDeleteShader(ShaderID);
    }
}
