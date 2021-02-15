#pragma once
#include <filesystem>
#include <glm/glm.hpp>

namespace gpgpu
{
  class Image
  {
  public:
    bool Load(const std::filesystem::path &path);

    inline const glm::ivec2 &GetSize() const { return m_size; }
    inline const std::vector<float> &GetData() const { return m_data; }

  private:
    glm::ivec2 m_size;
    std::vector<float> m_data;
  };
} // namespace gpgpu