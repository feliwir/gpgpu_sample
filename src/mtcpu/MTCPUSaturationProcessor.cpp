#include "MTCPUSaturationProcessor.hpp"
#include <glm/glm.hpp>
#include "../common/Saturation.hpp"
#include <algorithm>
#include <execution>

gpgpu::Image &gpgpu::MTCPUSaturationProcessor::Process(const Image &in)
{
  m_output.Resize(in.GetSize());

  // std::transform(std::execution::par_unseq, in.GetData().begin(), in.GetData().end(), m_output.GetData().begin(),
  //                [this](glm::vec4 inPixel) {
  //                  return Saturation::Apply(inPixel, m_factor);
  //                });

  #pragma omp parallel for
  for (int i = 0; i < in.GetSize().x * in.GetSize().y; i++)
  {
    const auto &inPixel = in.GetData()[i];
    auto &outPixel = m_output.GetData()[i];

    outPixel = Saturation::Apply(inPixel, m_factor);
  }

  return m_output;
}