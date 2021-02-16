#include "SyclBlurProcessor.hpp"

gpgpu::Image &gpgpu::SyclBlurProcessor::Process(const Image &in)
{
  return m_output;
}