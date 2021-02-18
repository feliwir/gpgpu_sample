#include "SYCLToneMappingProcessor.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include "../common/ToneMapping.hpp"
#include "../SYCLImage.hpp"

struct ToneMappingKernel
{
  void operator()(cl::sycl::item<1> item)
  {
    m_out_acc[item] = gpgpu::ToneMapping::Apply(m_in_acc[item], m_gamma, m_exposure);
  }

  float m_gamma;
  float m_exposure;
  sycl::accessor<glm::vec4, 1, sycl::access::mode::read, sycl::access::target::global_buffer> m_in_acc;
  sycl::accessor<glm::vec4, 1, sycl::access::mode::write, sycl::access::target::global_buffer> m_out_acc;
};

gpgpu::SYCLToneMappingProcessor::SYCLToneMappingProcessor(cl::sycl::queue &queue) : m_queue(queue)
{
  m_output = std::make_shared<SYCLImage>(m_queue);
}

void gpgpu::SYCLToneMappingProcessor::Process(std::shared_ptr<IImage> in)
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

      ToneMappingKernel fn{m_gamma, m_exposure, in_acc, out_acc};
      cgh.parallel_for(sycl::range<1>(num_pixels), fn);
    });

    process.wait();
  }
  catch (const cl::sycl::exception &e)
  {
    std::cout << e.what();
  }
}