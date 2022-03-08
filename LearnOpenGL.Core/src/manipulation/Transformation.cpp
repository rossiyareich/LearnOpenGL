#include "transformation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace manipulation
{
    glm::mat4 Transformation::TransformationMatrix(const glm::vec3& translate,
                                                   const Rotation& rotation,
                                                   const glm::vec3& scale)
    {
        auto trans = glm::mat4(1.0); // identity
        trans = glm::translate(trans, translate);
        trans = rotate(trans, rotation.Radians, rotation.Axis);
        trans = glm::scale(trans, scale);
        return trans;
    }
}
