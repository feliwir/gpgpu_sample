#include "SYCLBrightnessProcessor.hpp"
#include "../common/Brightness.hpp"
#include "../SYCLImage.hpp"
#include <iostream>

struct KernelFunctor
{
  void operator()(cl::sycl::item<1> item)
  {
    m_out_acc[item] = gpgpu::Brightness::Apply(m_in_acc[item], m_factor);
  }

  float m_factor;
  sycl::accessor<glm::vec4, 1, sycl::access::mode::read, sycl::access::target::global_buffer> m_in_acc;
  sycl::accessor<glm::vec4, 1, sycl::access::mode::write, sycl::access::target::global_buffer> m_out_acc;
};

void gpgpu::SYCLBrightnessProcessor::Process(std::shared_ptr<IImage> in)
{
  m_output->Resize(in->GetSize());

  // image buffer
  auto in_buf = std::dynamic_pointer_cast<SYCLImage>(in)->GetBuffer();
  auto out_buf = std::dynamic_pointer_cast<SYCLImage>(m_output)->GetBuffer();

  // submit command group on device
  try
  {
    auto process = m_queue.submit([&](sycl::handler &cgh) {
      // get image access
      auto in_acc = in_buf.get_access<sycl::access::mode::read>(cgh);
      auto out_acc = out_buf.get_access<sycl::access::mode::write>(cgh);

      const int num_pixels = in->GetSize().x * in->GetSize().y;

      KernelFunctor fn{m_factor, in_acc, out_acc};
      cgh.parallel_for(sycl::range<1>(num_pixels), fn);
    });

    process.wait();
  }
  catch (const cl::sycl::exception &e)
  {
    std::cout << e.what();
  }
}