#include "PhongLightSource.h"

#include "../camera/Camera3D.h"

namespace lighting
{
    PhongLightSource::PhongLightSource(const camera::Camera3D& camera,
                                       glm::vec3& lightPosition,
                                       glm::vec3& lightColor,
                                       float ambient,
                                       float diffuse,
                                       float specular) :
        LightPosition(lightPosition),
        LightColor(lightColor),
        Ambient(ambient),
        Diffuse(diffuse),
        Specular(specular),
        camera(camera)
    {
    }

    void PhongLightSource::Emit(const rendering::ShaderProgram& program) const
    {
        program.SetUFVector3("cameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        program.SetUFVector3("light.lightPosition", LightPosition.x, LightPosition.y, LightPosition.z);
        program.SetUFVector3("light.lightColor", LightColor.x, LightColor.y, LightColor.z);
        program.SetUFFloat("light.ambient", Ambient);
        program.SetUFFloat("light.diffuse", Diffuse);
        program.SetUFFloat("light.specular", Specular);
    }
}
