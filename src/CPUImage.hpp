#pragma once
#include "IImage.hpp"

namespace gpgpu
{
class CPUImage : public IImage
{
  public:
    static std::shared_ptr<IImage> From(std::shared_ptr<IImage> src);

    // Inherited
    std::vector<glm::vec4> &GetData() override;
    void Create(const glm::ivec2 &size, const std::vector<glm::vec4> &data) override;
    inline void Resize(const glm::ivec2 &size) override
    {
        m_size = size;
        if (size.x * size.y > 0)
        {
            m_data.resize(size.x * size.y);
        }
        else
        {
            m_data.clear();
        }
    }

  private:
    std::vector<glm::vec4> m_data;
};
} // namespace gpgpu