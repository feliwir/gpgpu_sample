#include "SYCLSaturationProcessor.hpp"
#include "SYCLSaturationKernel.hpp"
#include <CL/sycl.hpp>
#include <iostream>

gpgpu::Image &gpgpu::SYCLSaturationProcessor::Process(const Image &in)
{
  m_output.Resize(in.GetSize());

  // image buffer
  auto in_buf = sycl::buffer<glm::vec4, 1>(in.GetData().data(), sycl::range<1>(in.GetData().size()));
  auto out_buf = sycl::buffer<glm::vec4, 1>(m_output.GetData().data(), sycl::range<1>(m_output.GetData().size()));

  // submit command group on device
  try
  {
    auto process = m_queue.submit([&](sycl::handler &cgh) {
      // get image access
      auto in_acc = in_buf.get_access<sycl::access::mode::read>(cgh);
      auto out_acc = out_buf.get_access<sycl::access::mode::write>(cgh);

      const int num_pixels = in.GetSize().x * in.GetSize().y;

      KernelFunctor fn{m_factor, in_acc, out_acc};
      cgh.parallel_for(sycl::range<1>(num_pixels), fn);
    });

    process.wait();
  }
  catch (const cl::sycl::exception &e)
  {
    std::cout << e.what();
  }

  return m_output;
}