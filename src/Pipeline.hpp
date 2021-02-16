#pragma once
#include <vector>
#include <memory>
#include "IProcessor.hpp"

namespace gpgpu
{
  class Pipeline
  {
  public:
    inline void AddProcessor(std::shared_ptr<IProcessor> p)
    {
      m_processors.emplace_back(p);
    }

    inline void Process(const Image &img)
    {
      if (m_processors.empty())
      {
        return;
      }

      m_processors.front()->Process(img);
      for (int i = 1; i < m_processors.size(); i++)
      {
        m_processors[i]->Process(m_processors[i - 1]->GetOutput());
      }
    }

    inline bool Empty()
    {
      return m_processors.empty();
    }

    inline Image &GetOutput()
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