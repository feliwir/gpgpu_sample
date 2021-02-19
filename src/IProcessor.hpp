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

    inline void SetActive(bool active)
    {
        m_active = active;
    }
    inline bool GetActive() const
    {
        return m_active;
    }

  protected:
    bool m_active = true;
    std::shared_ptr<IImage> m_output;
};
} // namespace gpgpu