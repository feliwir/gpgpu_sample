#include "SYCLImage.hpp"
#include <iostream>

std::shared_ptr<gpgpu::IImage> gpgpu::SYCLImage::From(std::shared_ptr<gpgpu::IImage> src)
{
  // Input is null
  if (src == nullptr)
  {
    return std::make_shared<SYCLImage>();
  }

  // Already correct type
  if (std::dynamic_pointer_cast<SYCLImage>(src))
  {
    return src;
  }

  auto result = std::make_shared<SYCLImage>();
  result->Create(src->GetSize(), src->GetData());
  return result;
}

void gpgpu::SYCLImage::Create(const glm::ivec2 &size, const std::vector<glm::vec4> &data)
{
  if (data.size() != size.x * size.y)
    std::cerr << "Size missmatch" << std::endl;
}

std::vector<glm::vec4> &gpgpu::SYCLImage::GetData()
{
  std::vector<glm::vec4> bla;
  return bla;
}

void gpgpu::SYCLImage::Resize(const glm::ivec2& size)
{
  m_size = size;
}