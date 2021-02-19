#pragma once
#include "IImage.hpp"
#include "IProcessor.hpp"
#include <memory>
#include <vector>

namespace gpgpu
{
class Pipeline
{
  public:
    inline void AddProcessor(std::shared_ptr<IProcessor> p)
    {
        m_processors.emplace_back(p);
    }

    inline void Process(std::shared_ptr<IImage> img)
    {
        if (m_processors.empty())
        {
            return;
        }

        auto prevOutput = img;
        for (int i = 0; i < m_processors.size(); i++)
        {
            auto current = m_processors[i];
            if (!current->GetActive())
                continue;

            current->Process(prevOutput);
            prevOutput = current->GetOutput();
        }
    }

    inline bool Empty()
    {
        return m_processors.empty();
    }

    inline std::shared_ptr<IImage> GetOutput()
    {
        return m_processors.back()->GetOutput();
    }

    inline void Clear()
    {
        m_processors.clear();
    }

  private:
    std::vector<std::shared_ptr<IProcessor>> m_processors;
};
} // namespace gpgpu