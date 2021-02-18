#pragma once
#include "IImage.hpp"
#include <CL/sycl.hpp>

namespace gpgpu
{
  class SYCLImage : public IImage
  {
  public:
    inline SYCLImage(sycl::queue &queue) : m_queue(queue)
    {
    }

    static std::shared_ptr<IImage> From(std::shared_ptr<IImage> src, sycl::queue& queue);

    inline sycl::buffer<glm::vec4, 1> &GetBuffer()
    {
      return m_dev_buffer;
    }

    // Inherited
    virtual std::vector<glm::vec4> &GetData() override;
    virtual void Create(const glm::ivec2 &size, const std::vector<glm::vec4> &data) override;
    virtual void Resize(const glm::ivec2 &size) override;

  private:
    std::vector<glm::vec4> m_host_data;
    sycl::queue &m_queue;
    sycl::buffer<glm::vec4, 1> m_dev_buffer;
  };
} // namespace gpgpu