#pragma once
#include <glm/vec3.hpp>

namespace lighting
{
    struct PhongLightData
    {
        PhongLightData(const glm::vec3& lightDirection = {},
                       const glm::vec3& lightPosition = {},
                       float lightInnerCutoff = 0.0f,
                       float lightOuterCutoff = 0.0f);
        PhongLightData(glm::vec3& lightDirection,
                       glm::vec3& lightPosition,
                       float lightInnerCutoff = 0.0f,
                       float lightOuterCutoff = 0.0f);
        glm::vec3& LightDirection;
        glm::vec3& LightPosition;
        float LightInnerCutoff;
        float LightOuterCutoff;
    };
}
