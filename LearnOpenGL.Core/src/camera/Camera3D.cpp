#include "Camera3D.h"

#include <glm/ext/matrix_transform.hpp>

#include "../helpers/Constants.h"

#include "../manipulation/Rotation.h"

using Rot = manipulation::Rotation;

namespace camera
{
    Camera3D::Camera3D(glm::vec3 position, glm::vec3 front, glm::vec3 up) :
        Position{position},
        front{front},
        up{up}
    {
        SetEulerAngles(Rot::ToRadians(0.0f), Rot::ToRadians(0.0f), Rot::ToRadians(0.0f));
    }

    glm::mat4 Camera3D::GetView() const
    {
        return lookAt(Position, Position + front, up);
    }

    void Camera3D::MovePositionEuler(float x, float y, float z)
    {
        Position += (x * up) + (y * normalize(cross(front, up))) + (z * front);
    }

    void Camera3D::MovePosition(float x, float y, float z)
    {
        Position += (x * glm_vec3_up) + (y * normalize(cross(glm_vec3_front, glm_vec3_up))) + (z *
            glm_vec3_front);
    }

    void Camera3D::ChangeEulerAngles()
    {
        front.x = cos(yawRad) * cos(pitchRad);
        front.y = sin(pitchRad);
        front.z = sin(yawRad) * cos(pitchRad);
        up.x = cos(rollRad);
        up.y = sin(rollRad);
    }

    void Camera3D::SetEulerAngles(float pitchRad, float yawRad, float rollRad)
    {
        yawRad += Rot::ToRadians(-90.0f);
        rollRad += Rot::ToRadians(90.0f);

        this->pitchRad = pitchRad;
        this->yawRad = yawRad;
        this->rollRad = rollRad;

        ChangeEulerAngles();
    }

    void Camera3D::SetEulerAnglesRaw(float pitchRad, float yawRad, float rollRad)
    {
        this->pitchRad = pitchRad;
        this->yawRad = yawRad;
        this->rollRad = rollRad;

        ChangeEulerAngles();
    }

    void Camera3D::RotateEulerAngles(float pitchRad, float yawRad, float rollRad)
    {
        this->pitchRad += pitchRad;
        this->yawRad += yawRad;
        this->rollRad += rollRad;

        ChangeEulerAngles();
    }

    glm::vec3& Camera3D::GetFront()
    {
        return front;
    }

    glm::vec3& Camera3D::GetUp()
    {
        return up;
    }

    float& Camera3D::GetPitchRad()
    {
        return pitchRad;
    }

    float& Camera3D::GetYawRad()
    {
        return yawRad;
    }

    float& Camera3D::GetRollRad()
    {
        return rollRad;
    }

    glm::vec3& Camera3D::GetPosition()
    {
        return Position;
    }
}
