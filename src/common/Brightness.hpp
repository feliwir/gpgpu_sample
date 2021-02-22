#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>

namespace gpgpu
{
class Brightness
{
  public:
    static inline glm::vec3 Apply(const glm::vec3 &inPixel, float factor)
    {
        auto hsvPixel = glm::hsvColor(inPixel);

        hsvPixel.b = glm::clamp(factor * hsvPixel.b + hsvPixel.b, 0.0f, 1.0f);

        return glm::rgbColor(hsvPixel);
    }
};
} // namespace gpgpu