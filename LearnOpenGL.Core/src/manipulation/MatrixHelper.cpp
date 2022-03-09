#include "MatrixHelper.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace manipulation
{
    glm::mat4 MatrixHelper::TransformationMatrix(const glm::vec3& translate,
                                                 const Rotation& rotation,
                                                 const glm::vec3& scale)
    {
        auto trans = glm::mat4(1.0); // identity
        trans = glm::translate(trans, translate);
        trans = rotate(trans, rotation.Radians, rotation.Axis);
        trans = glm::scale(trans, scale);
        return trans;
    }

    glm::mat4 MatrixHelper::PerspectiveMatrix(const float fovRadians,
                                              const float ratio,
                                              const float near,
                                              const float far)
    {
        return glm::perspective(fovRadians, ratio, near, far);
    }

    glm::mat4 MatrixHelper::OrthographicMatrix(const float left,
                                               const float right,
                                               const float bottom,
                                               const float top,
                                               const float near,
                                               const float far)
    {
        return glm::ortho(left, right, bottom, top, near, far);
    }
}
