#include "PhongMaterial.h"

namespace lighting
{
    PhongMaterial::PhongMaterial(glm::vec3& objectColor,
                                 float ambient,
                                 float diffuse,
                                 float specular,
                                 uint32_t shininess) :
        ObjectColor(objectColor),
        Ambient(ambient),
        Diffuse(diffuse),
        Specular(specular),
        Shininess(shininess)
    {
    }

    void PhongMaterial::SendMaterial(const rendering::ShaderProgram& program) const
    {
        program.SetUFVector3("material.objectColor", ObjectColor.x, ObjectColor.y, ObjectColor.z);
        program.SetUFFloat("material.ambient", Ambient);
        program.SetUFFloat("material.diffuse", Diffuse);
        program.SetUFFloat("material.specular", Specular);
        program.SetUFUint32("material.shininess", Shininess);
    }
}
