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
        glm::vec3& LightPosition;
        glm::vec3& LightColor;
        float Ambient;
        float Diffuse;
        float Specular;

        PhongLightSource(const camera::Camera3D& camera,
                         glm::vec3& lightPosition,
                         glm::vec3& lightColor,
                         float ambient = 0.5f,
                         float diffuse = 1.0f,
                         float specular = 0.5f);

        void Emit(const rendering::ShaderProgram& program) const;
    private:
        const camera::Camera3D& camera;
    };
}
