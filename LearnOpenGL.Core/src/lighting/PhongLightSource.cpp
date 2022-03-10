#include "PhongLightSource.h"

#include "../camera/Camera3D.h"

namespace lighting
{
    PhongLightSource::PhongLightSource(const rendering::ShaderProgram& program,
                                       const rendering::VertexArray& array,
                                       glm::vec3* const worldPosition) :
        program{program},
        array{array},
        worldPosition{worldPosition},
        scene{nullptr}
    {
    }

    void PhongLightSource::DrawLightSource(const manipulation::RenderMatrix& pipeline,
                                           std::function<void()> drawCall) const
    {
        program.Use();
        array.Bind();
        pipeline.SetMatrixPipeline(program);
        drawCall();
        program.Unuse();
        array.Unbind();
    }

    void PhongLightSource::SetupScene(const rendering::ShaderProgram* const scene,
                                      const std::tuple<float, float, float>& lightColor)
    {
        this->scene = scene;
        auto& [r, g, b] = lightColor;
        scene->SetUFVector3("lightColor", r, g, b);
    }

    void PhongLightSource::FinishScene()
    {
        scene = nullptr;
    }

    void PhongLightSource::EmitAmbient(float intensity) const
    {
        scene->SetUFFloat("ambient", intensity);
    }

    void PhongLightSource::EmitDiffuse() const
    {
        scene->SetUFVector3("lightPosition", worldPosition->x, worldPosition->y, worldPosition->z);
    }

    void PhongLightSource::EmitSpecular(const camera::Camera3D& camera, float intensity) const
    {
        scene->SetUFVector3("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        scene->SetUFFloat("specular", intensity);
    }
}
