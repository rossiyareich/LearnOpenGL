#include "PhongLightSource.h"

#include <glm/trigonometric.hpp>

#include "PhongLightType.h"

#include "../camera/Camera3D.h"

namespace lighting
{
    PhongLightSource::PhongLightSource(const camera::Camera3D& camera,
                                       PhongLightData& lightData,
                                       glm::vec3& lightColor,
                                       PhongLightType lightType,
                                       float ambient,
                                       float diffuse,
                                       float specular) :
        LightData(lightData),
        LightColor(lightColor),
        Ambient(ambient),
        Diffuse(diffuse),
        Specular(specular),
        LightType(lightType),
        camera(camera)
    {
    }

    void PhongLightSource::Emit(const rendering::ShaderProgram& program) const
    {
        switch (LightType)
        {
        case PhongLightType::GlobalDirectional:
            program.SetUFVector3("light.lightData.lightDirection",
                                 LightData.LightDirection.x,
                                 LightData.LightDirection.y,
                                 LightData.LightDirection.z);
            break;
        case PhongLightType::Point:
            program.SetUFVector3("light.lightData.lightPosition",
                                 LightData.LightPosition.x,
                                 LightData.LightPosition.y,
                                 LightData.LightPosition.z);
            break;
        case PhongLightType::Spotlight:
            program.SetUFVector3("light.lightData.lightDirection",
                                 LightData.LightDirection.x,
                                 LightData.LightDirection.y,
                                 LightData.LightDirection.z);
            program.SetUFVector3("light.lightData.lightPosition",
                                 LightData.LightPosition.x,
                                 LightData.LightPosition.y,
                                 LightData.LightPosition.z);

            program.SetUFFloat("light.lightData.lightInnerCutoff", glm::cos(LightData.LightInnerCutoff));
            program.SetUFFloat("light.lightData.lightOuterCutoff", glm::cos(LightData.LightOuterCutoff));
            break;
        }

        program.SetUFVector3("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        program.SetUFUint32("light.lightType", static_cast<uint32_t>(LightType));
        program.SetUFVector3("light.lightColor", LightColor.x, LightColor.y, LightColor.z);
        program.SetUFFloat("light.ambient", Ambient);
        program.SetUFFloat("light.diffuse", Diffuse);
        program.SetUFFloat("light.specular", Specular);
    }
}
