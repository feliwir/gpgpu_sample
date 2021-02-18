#pragma once
#include "IImage.hpp"
#include <CL/sycl.hpp>

namespace gpgpu
{
  class SYCLImage : public IImage
  {
  public:
    static std::shared_ptr<IImage> From(std::shared_ptr<IImage> src);

    inline sycl::buffer<glm::vec4, 1> &GetBuffer()
    {
      return m_buffer;
    }

    // Inherited
    virtual std::vector<glm::vec4> &GetData() override;
    virtual void Create(const glm::ivec2 &size, const std::vector<glm::vec4> &data) override;
    virtual void Resize(const glm::ivec2 &size) override;

  private:
    sycl::buffer<glm::vec4, 1> m_buffer;
  };
} // namespace gpgpu