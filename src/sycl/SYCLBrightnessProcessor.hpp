#pragma once
#include "../IBrightnessProcessor.hpp"
#include <CL/sycl.hpp>

namespace gpgpu
{
class SYCLBrightnessProcessor : public IBrightnessProcessor
{
  public:
    SYCLBrightnessProcessor(cl::sycl::queue &queue);

    virtual void Process(std::shared_ptr<IImage> in) override;

  private:
    cl::sycl::queue &m_queue;
};
} // namespace gpgpu