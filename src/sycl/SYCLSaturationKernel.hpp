#pragma once
#include <CL/sycl.hpp>
#include <glm/glm.hpp>
#include "../common/Saturation.hpp"

struct KernelFunctor
{
  void operator()(cl::sycl::item<1> item)
  {
    m_out_acc[item] = gpgpu::Saturation::Apply(m_in_acc[item], m_factor);
  }

  float m_factor;
  sycl::accessor<glm::vec4, 1, sycl::access::mode::read, sycl::access::target::global_buffer> m_in_acc;
  sycl::accessor<glm::vec4, 1, sycl::access::mode::write, sycl::access::target::global_buffer> m_out_acc;
};
