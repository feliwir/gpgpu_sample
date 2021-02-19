#pragma once
#include "../IToneMappingProcessor.hpp"

namespace gpgpu
{
class MTCPUToneMappingProcessor : public IToneMappingProcessor
{
  public:
    MTCPUToneMappingProcessor();
    virtual void Process(std::shared_ptr<IImage> in) override;
};
} // namespace gpgpu