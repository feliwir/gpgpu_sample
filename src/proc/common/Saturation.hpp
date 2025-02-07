#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>

namespace gpgpu
{
class Saturation
{
  public:
    static inline glm::vec3 Apply(const glm::vec3 &inPixel, float factor)
    {
        auto hsvPixel = glm::hsvColor(inPixel);

        if (factor >= 0)
        {
            hsvPixel.g = hsvPixel.g + factor * (1 - hsvPixel.g) * hsvPixel.g;
        }
        else
        {
            hsvPixel.g = hsvPixel.g + factor * hsvPixel.g;
        }

        return glm::rgbColor(hsvPixel);
    }
};
} // namespace gpgpu