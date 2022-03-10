#pragma once
#include <glm/vec3.hpp>
#include "../rendering/ShaderProgram.h"

namespace lighting
{
    class PhongMaterial
    {
    public:
        glm::vec3& ObjectColor;
        float Ambient;
        float Diffuse;
        float Specular;
        uint32_t Shininess;

        PhongMaterial(glm::vec3& objectColor,
                      float ambient = 0.5f,
                      float diffuse = 1.0f,
                      float specular = 0.5f,
                      uint32_t shininess = 32);
        void SendMaterial(const rendering::ShaderProgram& program) const;
    };
}
