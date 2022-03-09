#include "RenderMatrix.h"

#include <glm/gtc/type_ptr.hpp>

namespace manipulation
{
    RenderMatrix::RenderMatrix(const glm::mat4& transformation,
                               const glm::mat4& view,
                               const glm::mat4& projection) :
        model{transformation},
        view{view},
        projection{projection}
    {
    }

    void RenderMatrix::SetMatrixPipeline(const rendering::ShaderProgram& program) const
    {
        program.SetUFMatrix4Float("model", value_ptr(model));
        program.SetUFMatrix4Float("view", value_ptr(view));
        program.SetUFMatrix4Float("projection", value_ptr(projection));
    }

    const glm::mat4& RenderMatrix::GetModel() const
    {
        return model;
    }

    const glm::mat4& RenderMatrix::GetView() const
    {
        return view;
    }

    const glm::mat4& RenderMatrix::GetProjection() const
    {
        return projection;
    }
}
