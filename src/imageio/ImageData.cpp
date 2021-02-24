#include "ImageData.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define NUM_IMG_CHANNELS 3

bool gpgpu::ImageData::Load(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path))
    {
        return false;
    }

    int width, height;
    auto data = stbi_loadf(path.c_str(), &width, &height, nullptr, NUM_IMG_CHANNELS);

    if (data == nullptr)
    {
        return false;
    }
    // Convert image data to floats
    m_data.resize(height * width);

    for (int i = 0; i < width * height; i++)
    {
        m_data[i].r = data[NUM_IMG_CHANNELS * i + 0];
        m_data[i].g = data[NUM_IMG_CHANNELS * i + 1];
        m_data[i].b = data[NUM_IMG_CHANNELS * i + 2];
        // m_data[i].a = data[NUM_IMG_CHANNELS * i + 3];
    }

    m_size = glm::ivec2(width, height);

    stbi_image_free(data);
    return true;
}