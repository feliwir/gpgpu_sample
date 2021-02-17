#include "SYCLBlurProcessor.hpp"

gpgpu::Image &gpgpu::SYCLBlurProcessor::Process(const Image &in)
{
  m_output = in;
  return m_output;
}