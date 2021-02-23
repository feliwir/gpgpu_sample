#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>

namespace gpgpu
{
class ToneMapping
{
  public:
    static inline glm::vec3 Apply(const glm::vec3 &inPixel, float gamma, float exposure)
    {
        glm::vec3 hdrColor = -glm::vec3(inPixel) * exposure;
        glm::vec3 expResult(glm::exp(hdrColor.r), glm::exp(hdrColor.g), glm::exp(hdrColor.b));
        glm::vec3 mapped = glm::vec3(1) - expResult;
        float invGamma = 1.0 / gamma;
        return glm::vec3(glm::pow(mapped.r, invGamma), glm::pow(mapped.g, invGamma), glm::pow(mapped.b, invGamma));
    }
};
} // namespace gpgpu