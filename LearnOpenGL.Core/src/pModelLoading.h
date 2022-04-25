#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "IMain.h"
#include "camera/Camera3D.h"
#include "chrono/Timer.h"
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
#include "lighting/PhongTexturedMaterial.h"

#include "logging/ConsoleLogger.h"
#include "logging/LogLevel.h"

#include "rendering/Texture.h"

#include "model/Model.h"

using namespace rendering;
using namespace window;
using namespace manipulation;
using namespace camera;
using namespace lighting;
using namespace logging;
using namespace model;

static float w = WINDOW_W;
static float h = WINDOW_H;

static float pitch{}, yaw{};
static double lastX{}, lastY{};
static bool firstMouse{true};

static void OnCursorPositionChanged(GLFWwindow* window, double xpos, double ypos)
{
    OnKeyReleasedPtr(window, GLFW_KEY_LEFT_CONTROL)
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
}

static float fov{45.0f};

static void OnScrollChanged(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= static_cast<float>(yoffset); // reverse because y is reversed
    if (fov > 45.0f)
        fov = 45.0f;
    if (fov < 1.0f)
        fov = 1.0f;
}

template <size_t TSize>
void EmitAllLights(const ShaderProgram& program, const std::array<std::pair<PhongLightSource, int>, TSize>& lights)
{
    for (const auto& light : lights)
    {
        auto& [source, index] = light;
        source.Emit(program, index);
    }
}

class pModelLoading : public IMain
{
public:
    int main() override
    {
        Window window{"LearnOpenGL", WINDOW_W, WINDOW_H};

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
        ConsoleLogger::Get().WriteLine(t_stringstream.str().c_str());

#pragma region OpenGL_Tuning
        //#define WIREFRAME

#ifdef WIREFRAME
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

        glEnable(GL_DEPTH_TEST);

#pragma endregion

        constexpr float vertices[]{
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0); // Use vertex attributes @ location = 0
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0); // Use vertex attributes @ location = 0
        lightSourceArray.Unbind();

        glm::vec3 lightColor{1.0f};
        std::array lightSources =
        {
            std::pair{
                PhongLightSource{
                    camera, {glm::vec3{1.0}}, glm::vec3{1.0f}, PhongLightType::GlobalDirectional, 0.1f, 0.0f, 0.0f
                },
                0
            },
            std::pair{PhongLightSource{camera, {{}, {0.7f, -5.0f, 2.0f}}, lightColor}, 0},
            std::pair{PhongLightSource{camera, {{}, {2.3f, -3.3f, -4.0f}}, lightColor}, 1},
            std::pair{PhongLightSource{camera, {{}, {-4.0f, 2.0f, -12.0f}}, lightColor}, 2},
            std::pair{PhongLightSource{camera, {{}, {0.0f, 5.0f, 0.0f}}, lightColor}, 3},
            std::pair{
                PhongLightSource{camera, {{0.0f, 1.0f, 1.0f}}, glm::vec3{1.0f}, PhongLightType::GlobalDirectional}, 1
            },
            std::pair{
                PhongLightSource{
                    camera,
                    {
                        camera.GetFront(),
                        camera.GetPosition(),
                        Rotation::ToRadians(20.0f),
                        Rotation::ToRadians(60.0f)
                    },
                    glm::vec3{0.5f, 0.5f, 0.5f},
                    PhongLightType::Spotlight,
                    0.0f,
                    4.0f,
                    2.0f
                },
                0
            }
        };

        const ShaderProgram& texturedProgram{
            {"src/ext/texturedVertexShader.vert", GL_VERTEX_SHADER},
            {"src/ext/texturedFragmentShader.frag", GL_FRAGMENT_SHADER}
        };
        const VertexArray& texturedArray{};
        texturedArray.Bind();
        vboBuffer.Bind();
        /* Set vbo vertex attributes */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0); // Use vertex attributes @ location = 0
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
        glEnableVertexAttribArray(1); // Use vertex attributes @ location = 1
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 6));
        glEnableVertexAttribArray(2); // Use vertex attributes @ location = 2
        texturedArray.Unbind();
        glm::vec3 texturedPositions[]{
            glm::vec3(0.0f, 0.0f, -15.0f),
            glm::vec3(2.0f, 5.0f, -14.0f),
            glm::vec3(-1.5f, -2.2f, -13.0f),
            glm::vec3(-3.8f, -2.0f, -12.0f),
            glm::vec3(2.4f, -0.4f, -11.0f),
            glm::vec3(-1.7f, 3.0f, -10.0f),
            glm::vec3(1.3f, -2.0f, -9.0f),
            glm::vec3(1.5f, 2.0f, -8.0f),
            glm::vec3(1.5f, 0.2f, -7.0f),
            glm::vec3(-1.3f, 1.0f, -6.0f)
        };
        PhongTexturedMaterial texturedMaterial{};
        const Texture& diffuseMap{"res/container2.png", GL_TEXTURE_2D, true};
        const Texture& specularMap{"res/container2_specular.png", GL_TEXTURE_2D, true};
        const Texture& emissionMap{"res/container2_emit.jpg", GL_TEXTURE_2D, true};

        ShaderProgram meshProgram{
            {"src/ext/meshVertexShader.vert", GL_VERTEX_SHADER},
            {"src/ext/meshFragmentShader.frag", GL_FRAGMENT_SHADER}
        };
        std::array models
        {
            std::make_pair<Model, glm::vec3>({"res/backpack/backpack.obj"}, {5.0f, 2.0f, 0.0f}),
            std::make_pair<Model, glm::vec3>({"res/monkey/monkey.obj"}, {10.0f, 2.0f, 0.0f}),
            std::make_pair<Model, glm::vec3>({"res/building_scene/building_scene.obj"}, {50.0f, 2.0f, 0.0f})
        };

        bool isLightOn{true}, isLightKeyToggle{true};

        chrono::Timer fpsUpdateTimer{};
        chrono::Timer deltaTimer{};

        fpsUpdateTimer.Start();
        deltaTimer.Start();

        float deltaTime{};
#define dt(var) (deltaTime * (var))
        while (!window.WindowShouldClose())
        {
            deltaTime = deltaTimer.GetElapsedF();
            deltaTimer.Start();

            if (fpsUpdateTimer.GetElapsed() > 1.0)
            {
                fpsUpdateTimer.Start();
                ConsoleLogger::Get().WriteLine(("FPS: " + std::to_string((1 / deltaTime))).c_str());
            }

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
            _OnKeyPressed(window, GLFW_KEY_L)
            {
                if (isLightKeyToggle)
                    isLightOn = !isLightOn;
                lightSources[6].first.Ambient = 0.0f;
                lightSources[6].first.Diffuse = isLightOn ? 4.0f : 0.0f;
                lightSources[6].first.Specular = isLightOn ? 2.0f : 0.0f;
                isLightKeyToggle = false;
            }
            _OnKeyReleased(window, GLFW_KEY_L)
                isLightKeyToggle = true;
            camera.SetEulerAnglesRaw(Rotation::ToRadians(pitch), Rotation::ToRadians(yaw - 90.0f), camera.GetRollRad());

            {
                auto [r, g, b]{HTMLToRGBFloat(0x000b24)};
                glClearColor(r, g, b, 1.0f);
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            {
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
                    glDrawArrays(GL_TRIANGLES, 0, carraysize(vertices) / 8);
                }
                // Do lighting
                cubeMaterial.SendMaterial(shaderProgram);
                EmitAllLights(shaderProgram, lightSources);
                // Finish drawing box
                shaderProgram.Unuse();
                vaoArray.Unbind();
            }

            {
                texturedProgram.Use();
                texturedArray.Bind();
                Texture::Activate(GL_TEXTURE0);
                diffuseMap.Bind();
                Texture::Activate(GL_TEXTURE1);
                specularMap.Bind();
                Texture::Activate(GL_TEXTURE2);
                emissionMap.Bind();

                for (auto& texturedPosition : texturedPositions)
                {
                    const RenderMatrix& texturedMatrixPipeline{
                        MatrixHelper::TransformationMatrix(texturedPosition),
                        camera.GetView(),
                        MatrixHelper::PerspectiveMatrix(
                            Rotation::ToRadians(fov),
                            w / h,
                            0.1f,
                            100.0f)
                    };
                    texturedMatrixPipeline.SetMatrixPipeline(texturedProgram);
                    glDrawArrays(GL_TRIANGLES, 0, carraysize(vertices) / 8);
                }

                // Do lighting
                texturedMaterial.SendMaterial(texturedProgram);
                EmitAllLights(texturedProgram, lightSources);
                texturedProgram.SetUFFloat("time", static_cast<float>(glfwGetTime()));

                diffuseMap.Unbind();
                specularMap.Unbind();
                texturedProgram.Unuse();
                texturedArray.Unbind();
            }

            {
                meshProgram.Use();

                for (auto& model : models)
                {
                    const RenderMatrix& meshMatrixPipeline{
                        MatrixHelper::TransformationMatrix(model.second),
                        camera.GetView(),
                        MatrixHelper::PerspectiveMatrix(
                            Rotation::ToRadians(fov),
                            w / h,
                            0.1f,
                            100.0f)
                    };
                    meshMatrixPipeline.SetMatrixPipeline(meshProgram);
                    model.first.Draw(meshProgram);
                }
                meshProgram.SetUFUint32("texturedMaterial.shininess", MATERIAL_SHININESS);
                EmitAllLights(meshProgram, lightSources);
                meshProgram.Unuse();
            }

            {
                // Start drawing light box
                lightSourceProgram.Use();
                lightSourceArray.Bind();
                for (auto& lightSource : lightSources)
                {
                    if (lightSource.first.LightType == PhongLightType::Point)
                    {
                        const RenderMatrix& lightMatrixPipeline{
                            MatrixHelper::TransformationMatrix(
                                lightSource.first.LightData.LightPosition,
                                Rotation({0.0f, 1.0f, 0.0f},
                                         static_cast<float>(glfwGetTime()) * 10.0f),
                                glm::vec3{0.3f}),
                            camera.GetView(),
                            MatrixHelper::PerspectiveMatrix(
                                Rotation::ToRadians(fov),
                                w / h,
                                0.1f,
                                100.0f)
                        };
                        lightMatrixPipeline.SetMatrixPipeline(lightSourceProgram);
                        glDrawArrays(GL_TRIANGLES, 0, carraysize(vertices) / 8);
                    }
                }
                // Finish drawing light box
                lightSourceProgram.Unuse();
                lightSourceArray.Unbind();
            }

            for (auto& lightSource : lightSources)
            {
                if (lightSource.first.LightType == PhongLightType::Point)
                {
                    lightSource.first.LightData.LightPosition.x = cos(static_cast<float>(glfwGetTime())) * 1.2f;
                    lightSource.first.LightData.LightPosition.z = sin(static_cast<float>(glfwGetTime())) * 1.2f;
                }
            }
            HSVtoRGB(lightColor.x, lightColor.y, lightColor.z, fmod(static_cast<float>(glfwGetTime()) * 10.0f, 360.0f),
                     0.3f,
                     1.0f);

            glfwSwapBuffers(window.Handle); /* Swap front and back buffers */
            glfwPollEvents(); /* Poll for and process events */
        }

        return 0;
    }
};
