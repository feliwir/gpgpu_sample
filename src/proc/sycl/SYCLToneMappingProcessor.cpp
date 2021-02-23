#include "SYCLToneMappingProcessor.hpp"
#include "../common/ToneMapping.hpp"
#include "SYCLImage.hpp"
#include <glm/glm.hpp>
#include <iostream>

gpgpu::SYCLToneMappingProcessor::SYCLToneMappingProcessor(cl::sycl::queue &queue) : m_queue(queue)
{
    m_output = std::make_shared<SYCLImage>(m_queue);
}

class ToneMappingKernel;

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
            float gamma = m_gamma;
            float exposure = m_exposure;

            cgh.parallel_for<ToneMappingKernel>(sycl::range<1>(num_pixels), [=](sycl::item<1> item) {
                out_acc[item] = gpgpu::ToneMapping::Apply(in_acc[item], gamma, exposure);
            });
        });
    }
    catch (const cl::sycl::exception &e)
    {
        std::cout << e.what();
    }
}