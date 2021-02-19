#pragma once
#include "../ISaturationProcessor.hpp"

namespace gpgpu
{
class MTCPUSaturationProcessor : public ISaturationProcessor
{
  public:
    MTCPUSaturationProcessor();
    void Process(std::shared_ptr<IImage> in) override;
};
} // namespace gpgpu