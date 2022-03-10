#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "camera/Camera3D.h"
#include "helpers/Constants.h"
#include "helpers/Helpers.h"
#include "manipulation/MatrixHelper.h"
#include "manipulation/RenderMatrix.h"
#include "rendering/Buffer.h"
#include "rendering/Shader.h"
#include "rendering/ShaderProgram.h"
#include "rendering/VertexArray.h"
#include "window/Window.h"
#include "lighting/PhongLightSource.h"
#include "lighting/PhongMaterial.h"

#include "logging/ConsoleLogger.h"
#include "logging/LogLevel.h"

using namespace rendering;
using namespace window;
using namespace manipulation;
using namespace camera;
using namespace lighting;
using namespace logging;

static float w = WINDOW_W;
static float h = WINDOW_H;

static float pitch{}, yaw{};
static double lastX{}, lastY{};
static bool firstMouse{true};

void OnCursorPositionChanged(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reverse because y is reversed
    lastX = xpos;
    lastY = ypos;

    xoffset *= MOUSE_SENSITIVITY;
    yoffset *= MOUSE_SENSITIVITY;

    yaw += static_cast<float>(xoffset);
    pitch += static_cast<float>(yoffset);

    if (pitch > 90.0f)
        pitch = 90.0f;
    if (pitch < -90.0f)
        pitch = -90.0f;
}

static float fov{45.0f};

void OnScrollChanged(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= static_cast<float>(yoffset); // reverse because y is reversed
    if (fov > 45.0f)
        fov = 45.0f;
    if (fov < 1.0f)
        fov = 1.0f;
}

int main()
{
    Window window{"LearnOpenGL", WINDOW_W, WINDOW_H};
    ConsoleLogger logger{};

    GLFWframebuffersizefun OnResized{
        [](GLFWwindow* window, int w, int h)
        {
            glViewport(0, 0, w, h);
            ::w = static_cast<float>(w);
            ::h = static_cast<float>(h);
        }
    };
    OnResized(window.Handle, WINDOW_W, WINDOW_H);
    glfwSetFramebufferSizeCallback(window.Handle, OnResized);

    GLFWwindowfocusfun OnFocused{
        [](GLFWwindow* window, int focused)
        {
            if (focused)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    };
    OnFocused(window.Handle, 1);
    glfwSetWindowFocusCallback(window.Handle, OnFocused);

    glfwSetCursorPosCallback(window.Handle, OnCursorPositionChanged);

    glfwSetScrollCallback(window.Handle, OnScrollChanged);

    std::stringstream t_stringstream{};
    t_stringstream << "OpenGL Version: " << glGetString(GL_VERSION);
    logger.WriteLine(t_stringstream.str().c_str());

#pragma region OpenGL_Tuning
    //#define WIREFRAME

#ifdef WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    glEnable(GL_DEPTH_TEST);

#pragma endregion

    // x, y, z, ..., normalX, normalY, normalZ
    constexpr float vertices[]{
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    // Create buffers
    const Buffer& vboBuffer{GL_ARRAY_BUFFER}; // Share cube buffer between 2 shaders
    vboBuffer.Bind();
    vboBuffer.PushArray(vertices, GL_STATIC_DRAW);
    vboBuffer.Unbind();

    Camera3D camera{{0.0f, 0.0f, 10.0f}};
    constexpr float moveSpeed{5.0f};
    constexpr float rollSpeed{1.0f};

    const ShaderProgram& shaderProgram{
        {"src/ext/vertexShader.vert", GL_VERTEX_SHADER},
        {"src/ext/fragmentShader.frag", GL_FRAGMENT_SHADER}
    };
    // Gen & setup vertex array {
    const VertexArray& vaoArray{};
    vaoArray.Bind();
    vboBuffer.Bind();
    /* Set vbo vertex attributes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0); // Use vertex attributes @ location = 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1); // Use vertex attributes @ location = 1
    vaoArray.Unbind();
    // } Unbind vertex array
    glm::vec3 cubePositions[]{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 4.0f, 0.0f)
    };
    glm::vec3 cubeColor{};
    {
        auto [r, g, b]{HTMLToRGBFloat(0xffffff)};
        cubeColor = {r, g, b};
    }
    PhongMaterial cubeMaterial{cubeColor};

    const ShaderProgram& lightSourceProgram{
        {"src/ext/lightSourceVertexShader.vert", GL_VERTEX_SHADER},
        {"src/ext/lightSourceFragmentShader.frag", GL_FRAGMENT_SHADER}
    };
    const VertexArray& lightSourceArray{};
    lightSourceArray.Bind();
    vboBuffer.Bind();
    /* Set vbo vertex attributes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0); // Use vertex attributes @ location = 0
    lightSourceArray.Unbind();
    glm::vec3 lightPosition{0.0f, 2.0f, 0.0f};
    glm::vec3 lightColor{1.0f};
    PhongLightSource lightSource{camera, lightPosition, lightColor};

    float deltaTime{}, lastFrame{};
    int fpsSampleCount{1};
#define dt(var) (deltaTime * (var))
    while (!window.WindowShouldClose())
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (fpsSampleCount % FPS_SAMPLE_RATE == 0)
        {
            fpsSampleCount = 0;
            logger.WriteLine(("FPS: " + std::to_string((1 / deltaTime))).c_str());
        }
        fpsSampleCount++;

        OnKeyPressed(window, GLFW_KEY_ESCAPE)
            window.SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _OnKeyPressed(window, GLFW_KEY_4)
        {
            fov = 45.0f;
            pitch = 0.0f;
            yaw = 0.0f;
            camera = Camera3D{{0.0f, 0.0f, 10.0f}};
        }
        _OnKeyPressed(window, GLFW_KEY_W)
            camera.MovePositionEuler(dt(moveSpeed), 0, 0);
        _OnKeyPressed(window, GLFW_KEY_A)
            camera.MovePositionEuler(0, -dt(moveSpeed), 0);
        _OnKeyPressed(window, GLFW_KEY_S)
            camera.MovePositionEuler(-dt(moveSpeed), 0, 0);
        _OnKeyPressed(window, GLFW_KEY_D)
            camera.MovePositionEuler(0, dt(moveSpeed), 0);
        _OnKeyPressed(window, GLFW_KEY_E)
            camera.RotateEulerAngles(0, 0, -dt(rollSpeed));
        _OnKeyPressed(window, GLFW_KEY_Q)
            camera.RotateEulerAngles(0, 0, dt(rollSpeed));
        _OnKeyPressed(window, GLFW_KEY_R)
            camera.MovePositionEuler(0, 0, dt(moveSpeed));
        _OnKeyPressed(window, GLFW_KEY_F)
            camera.MovePositionEuler(0, 0, -dt(moveSpeed));
        camera.SetEulerAnglesRaw(Rotation::ToRadians(pitch), Rotation::ToRadians(yaw - 90.0f), camera.GetRollRad());

        {
            auto [r, g, b]{HTMLToRGBFloat(0x000b24)};
            glClearColor(r, g, b, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start drawing box
        shaderProgram.Use();
        vaoArray.Bind();
        for (auto& cubePosition : cubePositions)
        {
            const RenderMatrix& cubeMatrixPipeline{
                MatrixHelper::TransformationMatrix(cubePosition, {}, {5.0f, 1.0f, 5.0f}),
                camera.GetView(),
                MatrixHelper::PerspectiveMatrix(
                    Rotation::ToRadians(fov),
                    w / h,
                    0.1f,
                    100.0f)
            };
            cubeMatrixPipeline.SetMatrixPipeline(shaderProgram);

            // Do lighting
            cubeMaterial.SendMaterial(shaderProgram);
            lightSource.Emit(shaderProgram);

            glDrawArrays(GL_TRIANGLES, 0, carraysize(vertices) / 6);
        }
        // Finish drawing box
        shaderProgram.Unuse();
        vaoArray.Unbind();

        lightPosition.x = cos(static_cast<float>(glfwGetTime())) * 3.0f;
        lightPosition.z = sin(static_cast<float>(glfwGetTime())) * 3.0f;

        HSVtoRGB(lightColor.x, lightColor.y, lightColor.z, fmod(static_cast<float>(glfwGetTime()) * 10.0f, 360.0f), 1,
                 1);

        // Start drawing light box
        lightSourceProgram.Use();
        lightSourceArray.Bind();
        const RenderMatrix& lightMatrixPipeline{
            MatrixHelper::TransformationMatrix(
                lightPosition,
                Rotation({0.0f, 1.0f, 0.0f},
                         static_cast<float>(glfwGetTime()) * 10.0f),
                glm::vec3{0.5f}),
            camera.GetView(),
            MatrixHelper::PerspectiveMatrix(
                Rotation::ToRadians(fov),
                w / h,
                0.1f,
                100.0f)
        };
        lightMatrixPipeline.SetMatrixPipeline(lightSourceProgram);
        glDrawArrays(GL_TRIANGLES, 0, carraysize(vertices) / 6);
        // Finish drawing light box
        lightSourceProgram.Unuse();
        lightSourceArray.Unbind();


        glfwSwapBuffers(window.Handle); /* Swap front and back buffers */
        glfwPollEvents(); /* Poll for and process events */
    }
}
