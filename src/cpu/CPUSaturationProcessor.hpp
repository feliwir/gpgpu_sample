#pragma once
#include "../ISaturationProcessor.hpp"

namespace gpgpu
{
  class CPUSaturationProcessor : public ISaturationProcessor
  {
  public:
    CPUSaturationProcessor();
    virtual void Process(std::shared_ptr<IImage> in) override;
  };
} // namespace gpgpu