#include "CPUToneMappingProcessor.hpp"
#include <glm/glm.hpp>
#include "../common/ToneMapping.hpp"
#include "../CPUImage.hpp"

gpgpu::CPUToneMappingProcessor::CPUToneMappingProcessor()
{
  m_output = std::make_shared<CPUImage>();
}

void gpgpu::CPUToneMappingProcessor::Process(std::shared_ptr<IImage> in)
{
  m_output->Resize(in->GetSize());

  for (int i = 0; i < in->GetSize().x * in->GetSize().y; i++)
  {
    const auto &inPixel = in->GetData()[i];
    auto &outPixel = m_output->GetData()[i];

    outPixel = ToneMapping::Apply(inPixel, m_gamma, m_exposure);
  }
}