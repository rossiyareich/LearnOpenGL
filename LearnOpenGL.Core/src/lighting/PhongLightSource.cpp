#include "PhongLightSource.h"

#include <sstream>

#include <glm/trigonometric.hpp>

#include "PhongLightType.h"

#include "../camera/Camera3D.h"

namespace lighting
{
    PhongLightSource::PhongLightSource(camera::Camera3D& camera,
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

    PhongLightSource::PhongLightSource(camera::Camera3D& camera,
                                       const PhongLightData& lightData,
                                       const glm::vec3& lightColor,
                                       PhongLightType lightType,
                                       float ambient,
                                       float diffuse,
                                       float specular) :
        LightData(const_cast<PhongLightData&>(lightData)),
        LightColor(const_cast<glm::vec3&>(lightColor)),
        Ambient(ambient),
        Diffuse(diffuse),
        Specular(specular),
        LightType(lightType),
        camera(camera)
    {
    }


#define putsH(value) (head.str() + (value)).c_str()

    void PhongLightSource::Emit(const rendering::ShaderProgram& program, int index) const
    {
        std::stringstream head{};
        switch (LightType)
        {
        case PhongLightType::GlobalDirectional:
            head << "globalDirLights[" << index << "]";
            program.SetUFVector3(putsH(".lightData.lightDirection"),
                                 LightData.LightDirection.x,
                                 LightData.LightDirection.y,
                                 LightData.LightDirection.z);
            break;
        case PhongLightType::Point:
            head << "pointLights[" << index << "]";
            program.SetUFVector3(putsH(".lightData.lightPosition"),
                                 LightData.LightPosition.x,
                                 LightData.LightPosition.y,
                                 LightData.LightPosition.z);
            break;
        case PhongLightType::Spotlight:
            head << "spotLights[" << index << "]";
            program.SetUFVector3(putsH(".lightData.lightDirection"),
                                 LightData.LightDirection.x,
                                 LightData.LightDirection.y,
                                 LightData.LightDirection.z);
            program.SetUFVector3(putsH(".lightData.lightPosition"),
                                 LightData.LightPosition.x,
                                 LightData.LightPosition.y,
                                 LightData.LightPosition.z);

            program.SetUFFloat(putsH(".lightData.lightInnerCutoff"), glm::cos(LightData.LightInnerCutoff));
            program.SetUFFloat(putsH(".lightData.lightOuterCutoff"), glm::cos(LightData.LightOuterCutoff));
            break;
        }

        program.SetUFVector3("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        program.SetUFUint32(putsH(".lightType"), static_cast<uint32_t>(LightType));
        program.SetUFVector3(putsH(".lightColor"), LightColor.x, LightColor.y, LightColor.z);
        program.SetUFFloat(putsH(".ambient"), Ambient);
        program.SetUFFloat(putsH(".diffuse"), Diffuse);
        program.SetUFFloat(putsH(".specular"), Specular);
    }
}
