#include "MTCPUSaturationProcessor.hpp"
#include "../CPUImage.hpp"
#include "../common/Saturation.hpp"
#include <glm/glm.hpp>

gpgpu::MTCPUSaturationProcessor::MTCPUSaturationProcessor()
{
    m_output = std::make_shared<CPUImage>();
}

void gpgpu::MTCPUSaturationProcessor::Process(std::shared_ptr<IImage> in)
{
    m_output->Resize(in->GetSize());

#pragma omp parallel for
    for (int i = 0; i < in->GetSize().x * in->GetSize().y; i++)
    {
        const auto &inPixel = in->GetData()[i];
        auto &outPixel = m_output->GetData()[i];

        outPixel = Saturation::Apply(inPixel, m_factor);
    }
}