#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>

namespace gpgpu
{
class Brightness
{
  public:
    static inline glm::vec4 Apply(const glm::vec4 &inPixel, float factor)
    {
        auto hsvPixel = glm::hsvColor(glm::vec3(inPixel));

        hsvPixel.b = glm::clamp(factor * hsvPixel.b + hsvPixel.b, 0.0f, 1.0f);

        return glm::vec4(glm::rgbColor(hsvPixel), inPixel.a);
    }
};
} // namespace gpgpu