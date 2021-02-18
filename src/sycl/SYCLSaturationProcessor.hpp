#pragma once
#include "../ISaturationProcessor.hpp"
#include <CL/sycl.hpp>

namespace gpgpu
{
  class SYCLSaturationProcessor : public ISaturationProcessor
  {
  public:
    inline SYCLSaturationProcessor(cl::sycl::queue &queue) : m_queue(queue)
    {
    }

    virtual void Process(std::shared_ptr<IImage> in) override;

  private:
    cl::sycl::queue &m_queue;
  };
} // namespace gpgpu