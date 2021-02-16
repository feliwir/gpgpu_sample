#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <memory>
#include <vector>
#include "Image.hpp"
#include "IBlurProcessor.hpp"
#include "IBrightnessProcessor.hpp"
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

    // Params
    float m_saturation = 0.0f;
    float m_brightness = 0.0f;

    int m_procTime = 0;
    std::shared_ptr<IBlurProcessor> m_blurProc;
    std::shared_ptr<IBrightnessProcessor> m_brightProc;
    std::shared_ptr<ISaturationProcessor> m_satProc;
  };
} // namespace gpgpu