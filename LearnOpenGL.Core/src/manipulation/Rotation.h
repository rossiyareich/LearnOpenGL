#pragma once
#include <glm/vec3.hpp>

namespace manipulation
{
    struct Rotation
    {
        Rotation();
        Rotation(glm::vec3 axis, float radians);
        static float ToRadians(float eulerAngles);
        const glm::vec3 Axis;
        const float Radians;
    };
}
