#pragma once
#include "IProcessor.hpp"
#include <glm/glm.hpp>

namespace gpgpu
{
  class IBrightnessProcessor : public IProcessor
  {
  public:
    virtual void SetFactor(float factor)
    {
      m_factor = glm::clamp(factor, -1.0f, 1.0f);
    }

  protected:
    float m_factor = 0.0f;
  };
} // namespace gpgpu