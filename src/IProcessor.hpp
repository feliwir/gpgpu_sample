#pragma once
#include "IImage.hpp"

namespace gpgpu
{
  class IProcessor
  {
  public:
    virtual void Process(std::shared_ptr<IImage> in) = 0;

    inline std::shared_ptr<IImage> GetOutput()
    {
      return m_output;
    }

  protected:
    std::shared_ptr<IImage> m_output;
  };
} // namespace gpgpu