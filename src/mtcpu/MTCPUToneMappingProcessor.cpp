#include "MTCPUToneMappingProcessor.hpp"
#include <glm/glm.hpp>
#include "../common/ToneMapping.hpp"
#include "../CPUImage.hpp"

gpgpu::MTCPUToneMappingProcessor::MTCPUToneMappingProcessor()
{
  m_output = std::make_shared<CPUImage>();
}

void gpgpu::MTCPUToneMappingProcessor::Process(std::shared_ptr<IImage> in)
{
  m_output->Resize(in->GetSize());

#pragma omp parallel for
  for (int i = 0; i < in->GetSize().x * in->GetSize().y; i++)
  {
    const auto &inPixel = in->GetData()[i];
    auto &outPixel = m_output->GetData()[i];

    outPixel = ToneMapping::Apply(inPixel, m_gamma, m_exposure);
  }
}