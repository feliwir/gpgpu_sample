#include "MTCPUBlurProcessor.hpp"

gpgpu::Image &gpgpu::MTCPUBlurProcessor::Process(const Image &in)
{
  m_output = in;
  return m_output;
}