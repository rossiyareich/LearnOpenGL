#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rotation.h"

namespace manipulation
{
    class Transformation
    {
    public:
        static glm::mat4 TransformationMatrix(const glm::vec3& translate = glm::vec3{},
                                              const Rotation& rotation = Rotation{},
                                              const glm::vec3& scale = glm::vec3{1.0f, 1.0f, 1.0f});
    };
}
