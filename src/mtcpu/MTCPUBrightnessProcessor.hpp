#pragma once
#include "../IBrightnessProcessor.hpp"

namespace gpgpu
{
class MTCPUBrightnessProcessor : public IBrightnessProcessor
{
  public:
    MTCPUBrightnessProcessor();
    virtual void Process(std::shared_ptr<IImage> in) override;
};
} // namespace gpgpu