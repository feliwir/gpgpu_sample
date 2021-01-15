#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <memory>
#include <vector>

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

    void UpdateBlur(int blur);

    GLuint m_glTexture;
    SDL_GLContext m_context;
    SDL_Window *m_window = nullptr;
    int m_width = 800;
    int m_height = 600;

    GLuint m_imageHandle = 0;
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    std::vector<float> m_imageData;
};
} // namespace volsycl