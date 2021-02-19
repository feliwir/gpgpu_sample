#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <memory>
#include <vector>
#include <CL/sycl.hpp>
#include "IImage.hpp"
#include "IBrightnessProcessor.hpp"
#include "ISaturationProcessor.hpp"
#include "IToneMappingProcessor.hpp"
#include "Pipeline.hpp"

namespace gpgpu
{
  class Window
  {
  public:
    Window();
    ~Window();
    void Run();

  private:
    void LoadImage();
    void UpdateImage();

    void CreatePipeline(int backend);

    void Reset();

    GLuint m_glTexture;
    SDL_GLContext m_context;
    SDL_Window *m_window = nullptr;
    int m_width = 800;
    int m_height = 600;

    Pipeline m_pipeline;
    GLuint m_imageHandle = 0;
    std::shared_ptr<IImage> m_input;
    std::string m_filename;

    // Params
    bool m_useTonemap = true;
    float m_exposure = 1.0f;
    float m_gamma = 2.0f;
    float m_saturation = 0.0f;
    float m_brightness = 0.0f;

    int m_procTime = 0;
    std::shared_ptr<IBrightnessProcessor> m_brightProc;
    std::shared_ptr<ISaturationProcessor> m_satProc;
    std::shared_ptr<IToneMappingProcessor> m_toneProc;

    // Sycl
    sycl::queue m_queue;
  };
} // namespace gpgpu