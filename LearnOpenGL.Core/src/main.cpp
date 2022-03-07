#define WIREFRAME

#include <cassert>
#include <iostream>
#include <ostream>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_W 800
#define WINDOW_H 600

#define GL_ERROR_LENGTH 512
using glErrorLog = char[GL_ERROR_LENGTH];

#define decltypearraymember(array) std::remove_all_extents<decltype(*(array))>::type

using namespace std::string_literals;

void WindowResizedCallback(GLFWwindow* const window, const int w, const int h)
{
    glViewport(0, 0, w, h);
}

void ProcessInputs(GLFWwindow* const window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

inline void ClearToHTMLColor(const uint32_t htmlColor, const float alpha)
{
    glClearColor(static_cast<float>((htmlColor & 0xFF0000) >> 16) / 255.0f,
                 static_cast<float>(((htmlColor & 0xFF00) >> 8)) / 255.0f,
                 static_cast<float>((htmlColor & 0xFF)) / 255.0f,
                 alpha);
}

GLuint CreateShader(const GLenum type, const char** code)
{
    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, code, nullptr);

    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glErrorLog infoLog;
        glGetShaderInfoLog(shader, GL_ERROR_LENGTH, nullptr, infoLog);
        std::cout << infoLog << std::endl;
        assert(false);
    }

    return shader;
}

void LinkShaderProgram(const GLuint program)
{
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glErrorLog infoLog;
        glGetProgramInfoLog(program, GL_ERROR_LENGTH, nullptr, infoLog);
        std::cout << infoLog << std::endl;
        assert(false);
    }
}

int main()
{
#pragma region Window
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_W, WINDOW_H, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, WindowResizedCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    assert(glewInit() == GLEW_OK);
    WindowResizedCallback(window, WINDOW_W, WINDOW_H);

    std::cout << glGetString(GL_VERSION) << std::endl;
#pragma endregion
#pragma region OpenGL Tuning
#ifdef WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
#pragma endregion

    // x, y, z
    constexpr float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left 
    };

    constexpr uint8_t indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    auto vertexShaderCode = R"(
#version 440 core
layout(location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

    auto fragmentShaderCode = R"(
#version 440 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)";

    // Create shaders
    const GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, &vertexShaderCode);
    const GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, &fragmentShaderCode);

    // Create shader program
    const GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    LinkShaderProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Gen buffers
    GLuint buffers[2]{};
    glGenBuffers((sizeof(buffers) / sizeof(decltypearraymember(buffers))), buffers);
    const GLuint vboBuffer = buffers[0];
    const GLuint eboBuffer = buffers[1];

    // Gen & bind vertex array {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Initialize pipeline
    // vbo
    glBindBuffer(GL_ARRAY_BUFFER, vboBuffer); //Bind vbo buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Push vertex to vbo buffer

    // ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Set vbo vertex attributes @ location = 0 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0); // Use vbo vertex attributes @ location = 0

    glBindVertexArray(0);
    // } Unbind vertex array

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ProcessInputs(window);

        ClearToHTMLColor(0x0f3b19, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Clear old buffer

        // Start drawing
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        /* Draw with active shader program, vbo, vbo attributes, ebo */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);

        // Finish drawing
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window); /* Swap front and back buffers */
        glfwPollEvents(); /* Poll for and process events */
    }

    // Cleanup
    glUseProgram(0);
    glDeleteProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, buffers);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
}
