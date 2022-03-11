#pragma once
#include <glm/vec3.hpp>

namespace lighting
{
    struct PhongLightData
    {
        PhongLightData();
        PhongLightData(const glm::vec3& lightDirection,
                       const glm::vec3& lightPosition,
                       float lightInnerCutoff,
                       float lightOuterCutoff);
        glm::vec3 LightDirection;
        glm::vec3 LightPosition;
        float LightInnerCutoff;
        float LightOuterCutoff;
    };
}
