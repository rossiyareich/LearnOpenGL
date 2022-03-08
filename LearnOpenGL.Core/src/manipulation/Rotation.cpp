#include "Rotation.h"

#include <glm/trigonometric.hpp>

namespace manipulation
{
    Rotation::Rotation() : Axis{glm::vec3{0.0f, 0.0f, 0.0f}}, Radians{0}
    {
    }

    Rotation::Rotation(const glm::vec3 axis, const float radians) : Axis{axis}, Radians{radians}
    {
    }

    float Rotation::ToRadians(const float eulerAngles)
    {
        return glm::radians(eulerAngles);
    }
}
