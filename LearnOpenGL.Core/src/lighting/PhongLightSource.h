#pragma once

#include <functional>

#include <glm/vec3.hpp>

#include "PhongLightData.h"
#include "PhongLightType.h"

#include "../rendering/ShaderProgram.h"
#include "../rendering/VertexArray.h"
#include "../manipulation/RenderMatrix.h"
#include "../camera/Camera3D.h"

namespace lighting
{
    class PhongLightSource
    {
    public:
        PhongLightData& LightData;
        glm::vec3& LightColor;
        float Ambient;
        float Diffuse;
        float Specular;
        PhongLightType LightType;

        PhongLightSource(camera::Camera3D& camera,
                         PhongLightData& lightData,
                         glm::vec3& lightColor,
                         PhongLightType lightType = PhongLightType::Point,
                         float ambient = 0.0f,
                         float diffuse = 1.0f,
                         float specular = 0.5f);

        PhongLightSource(camera::Camera3D& camera,
                         const PhongLightData& lightData,
                         const glm::vec3& lightColor,
                         PhongLightType lightType = PhongLightType::Point,
                         float ambient = 0.0f,
                         float diffuse = 1.0f,
                         float specular = 0.5f);

        void Emit(const rendering::ShaderProgram& program, int index) const;
    private:
        camera::Camera3D& camera;
    };
}
