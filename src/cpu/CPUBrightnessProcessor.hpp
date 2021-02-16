#pragma once
#include "../IBrightnessProcessor.hpp"

namespace gpgpu
{
  class CPUBrightnessProcessor : public IBrightnessProcessor
  {
  public:
    virtual Image &Process(const Image &in) override;
  };
} // namespace gpgpu