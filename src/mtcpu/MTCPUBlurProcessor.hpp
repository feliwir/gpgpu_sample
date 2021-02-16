#pragma once
#include "../IBlurProcessor.hpp"

namespace gpgpu
{
  class MTCPUBlurProcessor : public IBlurProcessor
  {
  public:
    virtual Image &Process(const Image &in) override;
  };
} // namespace gpgpu