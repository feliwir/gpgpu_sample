#include "CPUBrightnessProcessor.hpp"
#include <glm/glm.hpp>
#include "../common/Brightness.hpp"

gpgpu::Image &gpgpu::CPUBrightnessProcessor::Process(const Image &in)
{
  m_output.Resize(in.GetSize());

  for (int i = 0; i < in.GetSize().x * in.GetSize().y; i++)
  {
    const auto &inPixel = in.GetData()[i];
    auto &outPixel = m_output.GetData()[i];

    outPixel = Brightness::Apply(inPixel, m_factor);
  }
  return m_output;
}