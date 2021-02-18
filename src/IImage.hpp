#pragma once
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

namespace gpgpu
{
  class IImage
  {
  public:
    bool Load(const std::filesystem::path &path);

    inline const glm::ivec2 &GetSize() const { return m_size; }
    virtual std::vector<glm::vec4> &GetData() = 0;
    virtual void Create(const glm::ivec2 &size, const std::vector<glm::vec4> &data) = 0;
    virtual void Resize(const glm::ivec2 &size) = 0;

  protected:
    glm::ivec2 m_size = glm::ivec2(0);
  };
} // namespace gpgpu