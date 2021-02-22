#include "IImage.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

constexpr int NUM_IMG_CHANNELS = 3;

bool gpgpu::IImage::Load(const std::filesystem::path &path)
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
    std::vector<glm::vec3> m_data(height * width);

    for (int i = 0; i < width * height; i++)
    {
        m_data[i].r = data[NUM_IMG_CHANNELS * i + 0];
        m_data[i].g = data[NUM_IMG_CHANNELS * i + 1];
        m_data[i].b = data[NUM_IMG_CHANNELS * i + 2];
    }

    Create(glm::ivec2(width, height), m_data);

    stbi_image_free(data);
    return true;
}