#include "PhongLightData.h"

namespace lighting
{
    PhongLightData::PhongLightData(const glm::vec3& lightDirection,
                                   const glm::vec3& lightPosition,
                                   float lightInnerCutoff,
                                   float lightOuterCutoff):
        LightDirection{const_cast<glm::vec3&>(lightDirection)},
        LightPosition{const_cast<glm::vec3&>(lightPosition)},
        LightInnerCutoff{lightInnerCutoff},
        LightOuterCutoff{lightOuterCutoff}
    {
        assert(lightInnerCutoff <= lightOuterCutoff);
    }

    PhongLightData::PhongLightData(glm::vec3& lightDirection, glm::vec3& lightPosition, float lightInnerCutoff,
                                   float lightOuterCutoff)
        :
        LightDirection{lightDirection},
        LightPosition{lightPosition},
        LightInnerCutoff{lightInnerCutoff},
        LightOuterCutoff{lightOuterCutoff}
    {
    }
}
