#pragma once
#include "IProcessor.hpp"
#include <glm/glm.hpp>

namespace gpgpu
{
class IToneMappingProcessor : public IProcessor
{
  public:
    virtual void SetGamma(float gamma)
    {
        m_gamma = gamma;
    }

    virtual void SetExposure(float exposure)
    {
        m_exposure = exposure;
    }

  protected:
    float m_gamma = 2.0f;
    float m_exposure = 1.0f;
};
} // namespace gpgpu