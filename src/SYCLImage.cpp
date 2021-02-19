#include "SYCLImage.hpp"
#include <iostream>

std::shared_ptr<gpgpu::IImage> gpgpu::SYCLImage::From(std::shared_ptr<gpgpu::IImage> src, sycl::queue &queue)
{
    // Input is null
    if (src == nullptr)
    {
        return std::make_shared<SYCLImage>(queue);
    }

    // Already correct type
    if (std::dynamic_pointer_cast<SYCLImage>(src))
    {
        return src;
    }

    auto result = std::make_shared<SYCLImage>(queue);
    result->Create(src->GetSize(), src->GetData());
    return result;
}

void gpgpu::SYCLImage::Create(const glm::ivec2 &size, const std::vector<glm::vec4> &data)
{
    if (data.size() != size.x * size.y)
        std::cerr << "Size missmatch" << std::endl;

    Resize(size);

    if (m_size == glm::ivec2(0))
        return;

    auto in_buf = sycl::buffer<glm::vec4, 1>(data.data(), sycl::range<1>(data.size()));

    try
    {
        m_queue.submit([&](cl::sycl::handler &cgh) {
            auto in_acc = in_buf.get_access<sycl::access::mode::read>(cgh);
            auto out_acc = m_dev_buffer.get_access<sycl::access::mode::write>(cgh);
            cgh.copy(in_acc, out_acc);
        });
    }
    catch (const cl::sycl::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<glm::vec4> &gpgpu::SYCLImage::GetData()
{
    m_host_data.resize(m_size.x * m_size.y);

    if (m_host_data.empty())
        return m_host_data;

    auto out_buf = sycl::buffer<glm::vec4, 1>(m_host_data.data(), sycl::range<1>(m_host_data.size()));

    try
    {
        m_queue.submit([&](cl::sycl::handler &cgh) {
            auto in_acc = m_dev_buffer.get_access<sycl::access::mode::read>(cgh);
            auto out_acc = out_buf.get_access<sycl::access::mode::write>(cgh);
            cgh.copy(in_acc, out_acc);
        });
    }
    catch (const cl::sycl::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return m_host_data;
}

void gpgpu::SYCLImage::Resize(const glm::ivec2 &size)
{
    if (m_size == size)
        return;

    m_size = size;

    m_dev_buffer = sycl::buffer<glm::vec4, 1>(m_size.x * m_size.y);
}