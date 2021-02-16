#pragma once
#include "Image.hpp"

namespace gpgpu
{
  class IProcessor
  {
  public:
    virtual Image &Process(const Image &in) = 0;

    inline Image &GetOutput()
    {
      return m_output;
    }

  protected:
    Image m_output;
  };
} // namespace gpgpu