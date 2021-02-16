#include "CPUSaturationProcessor.hpp"
#include <glm/glm.hpp>
#include "../common/Saturation.hpp"

gpgpu::Image &gpgpu::CPUSaturationProcessor::Process(const Image &in)
{
  m_output.Resize(in.GetSize());

  for (int i = 0; i < in.GetSize().x * in.GetSize().y; i++)
  {
    const auto &inPixel = in.GetData()[i];
    auto &outPixel = m_output.GetData()[i];

    outPixel = Saturation::Apply(inPixel, m_factor);
  }
  return m_output;
}