#pragma once
#include "../IToneMappingProcessor.hpp"

namespace gpgpu
{
class CPUToneMappingProcessor : public IToneMappingProcessor
{
  public:
    CPUToneMappingProcessor();
    void Process(std::shared_ptr<IImage> in) override;
};
} // namespace gpgpu