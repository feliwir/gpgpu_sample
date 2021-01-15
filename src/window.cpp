#include "window.hpp"
#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyfiledialogs.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

constexpr int NUM_IMG_CHANNELS = 3;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam)
{
    if (type != GL_DEBUG_TYPE_ERROR)
        return;

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

gpgpu::Window::Window()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return;
    }

    // GL 3.2 Core + GLSL 150
    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow("gpgpu_sample", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    m_context = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    bool err = glewInit() != GLEW_OK;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

// See ImGuiFreeType::RasterizationFlags
#ifdef HAS_FREETYPE_SUPPORT
    unsigned int flags = ImGuiFreeType::NoHinting;
    ImGuiFreeType::BuildFontAtlas(io.Fonts, flags);
    std::cout << "Generated freetype font atlas" << std::endl;
#endif

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
}

gpgpu::Window::~Window()
{
    SDL_Quit();
}

void gpgpu::Window::LoadImage()
{
    std::vector<const char *> patterns;
    patterns.push_back("*.jpg");
    patterns.push_back("*.png");

    auto file = tinyfd_openFileDialog("Select an image", "", patterns.size(), patterns.data(), "Image files", 0);

    if(file)
    {
        auto data = stbi_loadf(file,&m_imageWidth, &m_imageHeight, nullptr, NUM_IMG_CHANNELS);

        // Convert image data to floats
        m_imageData.clear();
        m_imageData.resize(m_imageHeight* m_imageWidth * NUM_IMG_CHANNELS);

        for(int i=0;i<m_imageWidth * m_imageHeight;i++)
        {
            m_imageData[NUM_IMG_CHANNELS* i + 0] = data[NUM_IMG_CHANNELS* i + 0];
            m_imageData[NUM_IMG_CHANNELS* i + 1] = data[NUM_IMG_CHANNELS* i + 1];
            m_imageData[NUM_IMG_CHANNELS* i + 2] = data[NUM_IMG_CHANNELS* i + 2];
        }

        stbi_image_free(data);
        if(m_imageHandle == 0)
        {
            glGenTextures(1, &m_imageHandle);
        }

        UpdateImage();
    }
}

void gpgpu::Window::UpdateImage()
{
    glBindTexture(GL_TEXTURE_2D, m_imageHandle);
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_imageWidth, m_imageHeight, 0, GL_RGB, GL_FLOAT, m_imageData.data());
}

void gpgpu::Window::Run()
{
    glm::vec4 clearColor(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO &io = ImGui::GetIO();

    // Backend settings
    const char* backends[] = { "CPU", "Multicore CPU", "SYCL" };
    static int current_backend = 0;

    // Image settings
    float brightness = 0.0f;

    // Main loop
    bool render = false;
    bool done = false;
    bool drag = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
        // inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those
        // two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(m_window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(m_window);
        ImGui::NewFrame();

        // Image settings
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Image settings"); // Create a window called "Hello, world!" and append into it.
                if(ImGui::Button("Load image"))
                {
                    LoadImage();
                }
                ImGui::SliderFloat("Brightness", &brightness, -1.0f, 1.0f);
            ImGui::End();

            ImGui::Begin("Backend settings"); // Create a window called "Hello, world!" and append into it.

                ImGui::Combo("Backend API", &current_backend, backends, IM_ARRAYSIZE(backends));
            ImGui::End();

            ImGui::Begin("Image");
                int maxWidth = io.DisplaySize.x * 0.6f;
                int maxHeight = io.DisplaySize.y * 0.6f;

                int displayWidth = m_imageWidth;
                int displayHeight = m_imageHeight;
                
                if(displayWidth > maxWidth)
                {
                    float ratio = displayWidth / maxWidth;
                    displayWidth /= ratio;
                    displayHeight /= ratio;
                }

                if(displayHeight > maxHeight)
                {
                    float ratio = displayHeight / maxHeight;
                    displayWidth /= ratio;
                    displayHeight /= ratio;
                }

                ImGui::Image((void *)(intptr_t)m_imageHandle, ImVec2(displayWidth, displayHeight));
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_window);
    }
}