#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <memory>
#include <vector>
#include "Image.hpp"
#include "IBlurProcessor.hpp"
#include "ISaturationProcessor.hpp"
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

    GLuint m_glTexture;
    SDL_GLContext m_context;
    SDL_Window *m_window = nullptr;
    int m_width = 800;
    int m_height = 600;

    Pipeline m_pipeline;
    GLuint m_imageHandle = 0;
    Image m_input;

    std::shared_ptr<IBlurProcessor> m_blur;
    std::shared_ptr<ISaturationProcessor> m_saturation;
  };
} // namespace gpgpu