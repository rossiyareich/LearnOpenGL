#pragma once
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

#include "../helpers/Constants.h"

namespace camera
{
    class Camera3D
    {
    public:
        Camera3D(glm::vec3 position = glm::vec3{0.0f, 0.0f, 1.0f},
                 glm::vec3 front = glm_vec3_front,
                 glm::vec3 up = glm_vec3_up);

        glm::vec3 Position;

        void SetEulerAngles(float pitchRad, float yawRad, float rollRad);
        void RotateEulerAngles(float pitchRad, float yawRad, float rollRad);
        void MovePosition(float x, float y, float z);
        void MovePositionEuler(float x, float y, float z);

        glm::mat4 GetView() const;

        float GetPitchRad() const;
        float GetYawRad() const;
        float GetRollRad() const;
        const glm::vec3& GetPosition() const;
    private:
        void ChangeEulerAngles();
        glm::vec3 front;
        glm::vec3 up;
        float pitchRad;
        float yawRad;
        float rollRad;
    };
}
