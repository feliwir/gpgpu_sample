#include "SYCLBrightnessProcessor.hpp"
#include "../SYCLImage.hpp"
#include "../common/Brightness.hpp"
#include <iostream>

gpgpu::SYCLBrightnessProcessor::SYCLBrightnessProcessor(cl::sycl::queue &queue) : m_queue(queue)
{
    m_output = std::make_shared<SYCLImage>(m_queue);
}

class BrightnessKernel;

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
            float factor = m_factor;

            cgh.parallel_for<BrightnessKernel>(sycl::range<1>(num_pixels), [=](sycl::item<1> item) {
                out_acc[item] = gpgpu::Brightness::Apply(in_acc[item], factor);
            });
        });
    }
    catch (const cl::sycl::exception &e)
    {
        std::cout << e.what();
    }
}