#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <vector>
#include "MetaData.hpp"

namespace gpgpu
{
class ImageData
{
  public:
    inline const glm::ivec2 &GetSize() const
    {
        return m_size;
    }
    inline const std::vector<glm::vec3> &GetData() const
    {
      return m_data;
    }

    bool Load(const std::filesystem::path &path);

  private:
    glm::ivec2 m_size;
    MetaData m_meta;
    std::vector<glm::vec3> m_data;
};
}; // namespace gpgpu