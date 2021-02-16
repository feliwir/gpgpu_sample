#pragma once
#include "IProcessor.hpp"

namespace gpgpu
{
  class IBlurProcessor : public IProcessor
  {
  public:
    virtual void SetRadius(int radius)
    {
      m_radius = radius;
    }

  protected:
    int m_radius;
  };
} // namespace gpgpu