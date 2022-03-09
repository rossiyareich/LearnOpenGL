#pragma once
#include <glm/matrix.hpp>
#include "../rendering/ShaderProgram.h"

namespace manipulation
{
    class RenderMatrix
    {
    public:
        RenderMatrix(const glm::mat4& transformation,
                     const glm::mat4& view,
                     const glm::mat4& projection);

        void SetMatrixPipeline(const rendering::ShaderProgram& program) const;

        const glm::mat4& GetModel() const;
        const glm::mat4& GetView() const;
        const glm::mat4& GetProjection() const;
    private:
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };
}
