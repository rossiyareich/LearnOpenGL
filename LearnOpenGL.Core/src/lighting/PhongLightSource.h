#pragma once

#include <functional>

#include <glm/vec3.hpp>

#include "../rendering/ShaderProgram.h"
#include "../rendering/VertexArray.h"
#include "../manipulation/RenderMatrix.h"
#include "../camera/Camera3D.h"

namespace lighting
{
    class PhongLightSource
    {
    public:
        PhongLightSource(const rendering::ShaderProgram& program,
                         const rendering::VertexArray& array,
                         glm::vec3* worldPosition);
        void DrawLightSource(
            const manipulation::RenderMatrix& pipeline,
            std::function<void()> drawCall) const;
        void SetupScene(const rendering::ShaderProgram* scene, const std::tuple<float, float, float>& lightColor);
        void FinishScene();
        void EmitAmbient(float intensity) const;
        void EmitDiffuse() const;
        void EmitSpecular(const camera::Camera3D& camera, float intensity) const;
    private:
        const rendering::ShaderProgram& program;
        const rendering::VertexArray& array;
        glm::vec3* worldPosition;
        const rendering::ShaderProgram* scene;
    };
}
