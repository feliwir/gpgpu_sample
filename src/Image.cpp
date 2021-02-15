#include "Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool gpgpu::Image::Load(const std::filesystem::path &path)
{
  if (!std::filesystem::exists(path))
  {
    return false;
  }

  auto data = stbi_loadf(file, &m_imageWidth, &m_imageHeight, nullptr, NUM_IMG_CHANNELS);

  if (data == nullptr)
  {
    return false;
  }

  // Convert image data to floats
  m_data.resize(m_imageHeight * m_imageWidth * NUM_IMG_CHANNELS);

  for (int i = 0; i < m_imageWidth * m_imageHeight; i++)
  {
    m_data[NUM_IMG_CHANNELS * i + 0] = data[NUM_IMG_CHANNELS * i + 0];
    m_data[NUM_IMG_CHANNELS * i + 1] = data[NUM_IMG_CHANNELS * i + 1];
    m_data[NUM_IMG_CHANNELS * i + 2] = data[NUM_IMG_CHANNELS * i + 2];
  }

  stbi_image_free(data);
}