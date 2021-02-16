#pragma once
#include "../ISaturationProcessor.hpp"

namespace gpgpu
{
  class MTCPUSaturationProcessor : public ISaturationProcessor
  {
  public:
    virtual Image &Process(const Image &in) override;
  };
} // namespace gpgpu