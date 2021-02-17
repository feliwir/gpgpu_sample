#pragma once
#include "../IBrightnessProcessor.hpp"

namespace gpgpu
{
  class SYCLBrightnessProcessor : public IBrightnessProcessor
  {
  public:
    virtual Image &Process(const Image &in) override;
  };
} // namespace gpgpu