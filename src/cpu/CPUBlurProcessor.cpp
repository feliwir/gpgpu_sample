#include "CPUBlurProcessor.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>

gpgpu::Image &gpgpu::CPUBlurProcessor::Process(const Image &in)
{
  m_output = in;
  return m_output;
}