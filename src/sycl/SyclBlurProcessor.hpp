#pragma once
#include "../IBlurProcessor.hpp"

namespace gpgpu
{
  class SyclBlurProcessor : public IBlurProcessor
  {
  public:
    virtual Image &Process(const Image &in) override;
  };
} // namespace gpgpu