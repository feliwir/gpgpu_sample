#pragma once
#include "../ISaturationProcessor.hpp"

namespace gpgpu
{
  class CPUSaturationProcessor : public ISaturationProcessor
  {
  public:
    virtual Image &Process(const Image &in) override;
  };
} // namespace gpgpu