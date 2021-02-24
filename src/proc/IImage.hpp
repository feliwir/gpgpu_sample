#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <vector>

namespace gpgpu
{
class IImage
{
  public:
    inline const glm::ivec2 &GetSize() const
    {
        return m_size;
    }
    virtual std::vector<glm::vec3> &GetData() = 0;
    virtual void Create(const glm::ivec2 &size, const std::vector<glm::vec3> &data) = 0;
    virtual void Resize(const glm::ivec2 &size) = 0;

  protected:
    glm::ivec2 m_size = glm::ivec2(0);
};
} // namespace gpgpu