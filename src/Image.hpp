#pragma once
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

namespace gpgpu
{
  class Image
  {
  public:
    bool Load(const std::filesystem::path &path);

    inline void Resize(const glm::ivec2 &size)
    {
      m_size = size;
      if(size.x * size.y > 0)
        m_data.resize(size.x * size.y);
      else
        m_data.clear();
    }

    inline const glm::ivec2 &GetSize() const { return m_size; }
    inline std::vector<glm::vec4> &GetData() { return m_data; }
    inline const std::vector<glm::vec4> &GetData() const { return m_data; }

  private:
    glm::ivec2 m_size;
    std::vector<glm::vec4> m_data;
  };
} // namespace gpgpu