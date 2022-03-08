#include <functional>
#include <iostream>
#include <ostream>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "helpers/Constants.h"
#include "helpers/Helpers.h"

#include "manipulation/Transformation.h"

#include "rendering/Buffer.h"
#include "rendering/Shader.h"
#include "rendering/ShaderProgram.h"
#include "rendering/Texture.h"
#include "rendering/VertexArray.h"

#include "window/Window.h"

using namespace rendering;
using namespace window;
using namespace manipulation;

void ProcessInputs(const Window& window)
{
    if (glfwGetKey(window.Handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        window.SetWindowShouldClose(true);
}

inline std::tuple<float, float, float> HTMLToRGBFloat(const uint32_t htmlColor)
{
    return {
        static_cast<float>((htmlColor & 0xFF0000) >> 16) / 255.0f,
        static_cast<float>(((htmlColor & 0xFF00) >> 8)) / 255.0f,
        static_cast<float>((htmlColor & 0xFF)) / 255.0f
    };
}

int main()
{
    Window window{"LearnOpenGL", WINDOW_W, WINDOW_H};

    GLFWframebuffersizefun OnResized{
        [](GLFWwindow* window, int w, int h)
        {
            glViewport(0, 0, w, h);
        }
    };
    OnResized(window.Handle, WINDOW_W, WINDOW_H);
    glfwSetFramebufferSizeCallback(window.Handle, OnResized);

    std::cout << glGetString(GL_VERSION) << std::endl;

#pragma region OpenGL_Tuning
    //#define WIREFRAME

#ifdef WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

#pragma endregion

    // x, y, z /* */ r, g, b, a /* */ x, y
    constexpr float vertices[]{
        0.5f, 0.5f, 0.0f, /* */ 0.0f, 1.0f, 0.0f, 1.0f, /* */ 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, /* */ 1.0f, 1.0f, 0.0f, 1.0f, /* */ 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, /* */ 0.0f, 0.0f, 1.0f, 1.0f, /* */ 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, /* */ 1.0f, 0.0f, 0.0f, 1.0f /* */, 0.0f, 1.0f // top left 
    };

    constexpr uint8_t indices[]{
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    constexpr float texCoords[]{
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    // Create shader program
    Shader vertexShader{"src/ext/vertexShader.vert", GL_VERTEX_SHADER};
    Shader fragmentShader{"src/ext/fragmentShader.frag", GL_FRAGMENT_SHADER};
    const ShaderProgram& shaderProgram{vertexShader, fragmentShader};

    // Create buffers
    const Buffer& vboBuffer{GL_ARRAY_BUFFER};
    const Buffer& eboBuffer{GL_ELEMENT_ARRAY_BUFFER};

    // Gen & setup vertex array {
    const VertexArray& vaoArray{};
    vaoArray.Bind();

    vboBuffer.Bind();
    vboBuffer.PushArray(vertices, GL_STATIC_DRAW);
    eboBuffer.Bind();
    eboBuffer.PushArray(indices, GL_STATIC_DRAW);

    /* Set vbo vertex attributes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0); // Use vertex attributes @ location = 0
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1); // Use rgba attributes @ location = 1
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 7));
    glEnableVertexAttribArray(2); // Use texCoords attributes @ location = 2

    vaoArray.Unbind();
    // } Unbind vertex array

    // Setup textures
    const Texture& boxTexture{"res/container.jpg", GL_TEXTURE_2D, true};
    const Texture& awesomeFaceTexture{"res/awesomeface.png", GL_TEXTURE_2D, true};
    shaderProgram.Use();
    shaderProgram.SetUFInt("texture1", 0);
    shaderProgram.SetUFInt("texture2", 1);

    while (!window.WindowShouldClose())
    {
        ProcessInputs(window);

        auto [r, g, b]{HTMLToRGBFloat(0x0f3b19)};
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start drawing
        shaderProgram.Use();
        vaoArray.Bind();

        Texture::Activate(GL_TEXTURE0);
        boxTexture.Bind();
        Texture::Activate(GL_TEXTURE1);
        awesomeFaceTexture.Bind();

        const auto& transformationMat = Transformation::TransformationMatrix(
            {0.5f, -0.5f, 0.0f},
            {{0.0f, 0.0f, 1.0f}, static_cast<float>(glfwGetTime()) * -3.0f},
            {0.5f, 0.5f, 0.5f});
        shaderProgram.SetUFMatrix4Float("transformationMatrix", value_ptr(transformationMat));

        glDrawElements(GL_TRIANGLES, carraysize(indices), GL_UNSIGNED_BYTE, nullptr);

        boxTexture.Unbind();
        awesomeFaceTexture.Unbind();

        // Finish drawing
        shaderProgram.Unuse();
        vaoArray.Unbind();


        glfwSwapBuffers(window.Handle); /* Swap front and back buffers */
        glfwPollEvents(); /* Poll for and process events */
    }
}
