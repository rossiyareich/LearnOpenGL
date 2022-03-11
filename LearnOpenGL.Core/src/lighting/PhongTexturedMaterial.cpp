#include "PhongTexturedMaterial.h"

namespace lighting
{
    PhongTexturedMaterial::PhongTexturedMaterial(int diffuse,
                                                 int specular,
                                                 int emission,
                                                 uint32_t shininess) :
        Diffuse(diffuse),
        Specular(specular),
        Emission(emission),
        Shininess(shininess)
    {
    }

    void PhongTexturedMaterial::SendMaterial(const rendering::ShaderProgram& program) const
    {
        program.SetUFInt("diffuse", Diffuse);
        program.SetUFInt("specular", Specular);
        program.SetUFInt("emission", Emission);
        program.SetUFUint32("texturedMaterial.shininess", Shininess);
    }
}
