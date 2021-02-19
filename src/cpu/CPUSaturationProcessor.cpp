#include "CPUSaturationProcessor.hpp"
#include "../CPUImage.hpp"
#include "../common/Saturation.hpp"
#include <glm/glm.hpp>

gpgpu::CPUSaturationProcessor::CPUSaturationProcessor()
{
    m_output = std::make_shared<CPUImage>();
}

void gpgpu::CPUSaturationProcessor::Process(std::shared_ptr<IImage> in)
{
    m_output->Resize(in->GetSize());

    for (int i = 0; i < in->GetSize().x * in->GetSize().y; i++)
    {
        const auto &inPixel = in->GetData()[i];
        auto &outPixel = m_output->GetData()[i];

        outPixel = Saturation::Apply(inPixel, m_factor);
    }
}