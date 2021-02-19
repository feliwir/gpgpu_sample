#pragma once
#include "../IToneMappingProcessor.hpp"
#include <CL/sycl.hpp>

namespace gpgpu
{
class SYCLToneMappingProcessor : public IToneMappingProcessor
{
  public:
    SYCLToneMappingProcessor(cl::sycl::queue &queue);
    virtual void Process(std::shared_ptr<IImage> in) override;

  private:
    cl::sycl::queue &m_queue;
};
} // namespace gpgpu