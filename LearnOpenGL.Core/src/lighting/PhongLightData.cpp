#include "PhongLightData.h"

namespace lighting
{
    PhongLightData::PhongLightData() : LightDirection{}, LightPosition{}, LightInnerCutoff{}, LightOuterCutoff{}
    {
    }

    PhongLightData::PhongLightData(const glm::vec3& lightDirection,
                                   const glm::vec3& lightPosition,
                                   float lightInnerCutoff,
                                   float lightOuterCutoff):
        LightDirection{lightDirection},
        LightPosition{lightPosition},
        LightInnerCutoff{lightInnerCutoff},
        LightOuterCutoff{lightOuterCutoff}
    {
        assert(lightInnerCutoff <= lightOuterCutoff);
    }
}
