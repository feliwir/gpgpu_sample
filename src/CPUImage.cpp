#include "CPUImage.hpp"
#include <iostream>

std::vector<glm::vec3> &gpgpu::CPUImage::GetData()
{
    return m_data;
}

std::shared_ptr<gpgpu::IImage> gpgpu::CPUImage::From(std::shared_ptr<gpgpu::IImage> src)
{
    // Input is null
    if (src == nullptr)
    {
        return std::make_shared<CPUImage>();
    }

    // Already correct type
    if (std::dynamic_pointer_cast<CPUImage>(src))
    {
        return src;
    }

    auto result = std::make_shared<CPUImage>();
    result->Create(src->GetSize(), src->GetData());
    return result;
}

void gpgpu::CPUImage::Create(const glm::ivec2 &size, const std::vector<glm::vec3> &data)
{
    if (data.size() != size.x * size.y)
    {
        std::cerr << "Size missmatch" << std::endl;
    }
    m_size = size;
    m_data = data;
}