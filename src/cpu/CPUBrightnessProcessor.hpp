#pragma once
#include "../IBrightnessProcessor.hpp"

namespace gpgpu
{
class CPUBrightnessProcessor : public IBrightnessProcessor
{
  public:
    CPUBrightnessProcessor();
    virtual void Process(std::shared_ptr<IImage> in) override;
};
} // namespace gpgpu