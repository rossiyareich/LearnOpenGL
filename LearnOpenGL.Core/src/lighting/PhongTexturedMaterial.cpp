#include "PhongTexturedMaterial.h"

namespace lighting
{
    PhongTexturedMaterial::PhongTexturedMaterial(int diffuse,
                                                 int specular,
                                                 uint32_t shininess) :
        Diffuse(diffuse),
        Specular(specular),
        Shininess(shininess)
    {
    }

    void PhongTexturedMaterial::SendMaterial(const rendering::ShaderProgram& program) const
    {
        program.SetUFInt("diffuse", Diffuse);
        program.SetUFInt("specular", Specular);
        program.SetUFUint32("texturedMaterial.shininess", Shininess);
    }
}
