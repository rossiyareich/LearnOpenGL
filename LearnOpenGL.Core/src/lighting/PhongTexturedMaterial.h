﻿#pragma once
#include <glm/vec3.hpp>
#include "../rendering/ShaderProgram.h"

namespace lighting
{
    class PhongTexturedMaterial
    {
    public:
        int Diffuse;
        int Specular;
        int Emission;
        uint32_t Shininess;

        PhongTexturedMaterial(int diffuse = 0,
                              int specular = 1,
                              int emission = 2,
                              uint32_t shininess = 32);
        void SendMaterial(const rendering::ShaderProgram& program) const;
    };
}