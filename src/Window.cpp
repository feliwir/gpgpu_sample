#include "Window.hpp"
#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyfiledialogs.h>

// Blur
#include "cpu/CPUBlurProcessor.hpp"
#include "mtcpu/MTCPUBlurProcessor.hpp"
#include "sycl/SYCLBlurProcessor.hpp"

// Saturation
#include "cpu/CPUSaturationProcessor.hpp"
#include "mtcpu/MTCPUSaturationProcessor.hpp"
#include "sycl/SYCLSaturationProcessor.hpp"

// Brightness
#include "cpu/CPUBrightnessProcessor.hpp"
#include "mtcpu/MTCPUBrightnessProcessor.hpp"
#include "sycl/SYCLBrightnessProcessor.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam)
{
    if (type != GL_DEBUG_TYPE_ERROR)
        return;

    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

gpgpu::Window::Window() : m_queue(sycl::default_selector{})
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

    // Create pipeline
    CreatePipeline(0);

    // SYCL
    const auto &device = m_queue.get_device();
    std::cout << "Using device: " << device.get_info<sycl::info::device::name>() << std::endl;

    std::cout << "Image support: " << device.get_info<sycl::info::device::image_support>() << std::endl;
    std::cout << "Image3D width: " << device.get_info<sycl::info::device::image3d_max_width>() << std::endl;
    std::cout << "Image3D height: " << device.get_info<sycl::info::device::image3d_max_height>() << std::endl;
    std::cout << "Image3D depth: " << device.get_info<sycl::info::device::image3d_max_depth>() << std::endl;
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

    if (file)
    {
        if (m_input.Load(file))
        {
            UpdateImage();
        }
    }
}

void gpgpu::Window::UpdateImage()
{
    if (m_pipeline.Empty())
    {
        return;
    }

    std::cout << "Update image" << std::endl;
    auto beg = std::chrono::system_clock::now();
    m_pipeline.Process(m_input);
    auto end = std::chrono::system_clock::now();

    m_procTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count();
    std::cout << "Pipeline process: " << m_procTime << "ms" << std::endl;

    std::cout << "Update image" << std::endl;
    // Upload output
    if (m_imageHandle == 0)
    {
        glGenTextures(1, &m_imageHandle);
    }

    glBindTexture(GL_TEXTURE_2D, m_imageHandle);
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    Image &output = m_pipeline.GetOutput();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, output.GetSize().x, output.GetSize().y, 0, GL_RGBA, GL_FLOAT, output.GetData().data());
}

void gpgpu::Window::CreatePipeline(int backend)
{
    std::cout << "Creating pipeline: " << backend << std::endl;
    m_pipeline.Clear();

    switch (backend)
    {
    //CPU
    case 0:
        m_blurProc = std::make_shared<CPUBlurProcessor>();
        m_brightProc = std::make_shared<CPUBrightnessProcessor>();
        m_satProc = std::make_shared<CPUSaturationProcessor>();
        break;
    case 1:
        m_blurProc = std::make_shared<MTCPUBlurProcessor>();
        m_brightProc = std::make_shared<MTCPUBrightnessProcessor>();
        m_satProc = std::make_shared<MTCPUSaturationProcessor>();
        break;
    case 2:
        m_blurProc = std::make_shared<SYCLBlurProcessor>();
        m_brightProc = std::make_shared<SYCLBrightnessProcessor>();
        m_satProc = std::make_shared<SYCLSaturationProcessor>(m_queue);
        break;
    default:
        break;
    }

    m_satProc->SetFactor(m_saturation);

    m_pipeline.AddProcessor(m_brightProc);
    m_pipeline.AddProcessor(m_satProc);
}

void gpgpu::Window::Run()
{
    glm::vec4 clearColor(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO &io = ImGui::GetIO();

    // Backend settings
    const char *backends[] = {"CPU", "Multicore CPU", "SYCL"};
    static int current_backend = 0;

    // Image settings
    int blur = 1;

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
            if (ImGui::Button("Load image"))
                LoadImage();

            if (ImGui::SliderInt("Blur", &blur, 1, 10))
            {
                m_blurProc->SetRadius(blur);
                UpdateImage();
            }

            if (ImGui::SliderFloat("Saturation", &m_saturation, -1.0f, 1.0f))
            {
                m_satProc->SetFactor(m_saturation);
                UpdateImage();
            }

            if (ImGui::SliderFloat("Brightness", &m_brightness, -1.0f, 1.0f))
            {
                m_brightProc->SetFactor(m_brightness);
                UpdateImage();
            }

            ImGui::LabelText("Time", "Processed in %ums", m_procTime);
            ImGui::End();

            ImGui::Begin("Backend settings"); // Create a window called "Hello, world!" and append into it.

            if (ImGui::Combo("Backend API", &current_backend, backends, IM_ARRAYSIZE(backends)))
            {
                CreatePipeline(current_backend);
                UpdateImage();
            }
            ImGui::End();

            ImGui::Begin("Image", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            int maxWidth = io.DisplaySize.x * 0.8f;
            int maxHeight = io.DisplaySize.y * 0.8f;

            int displayWidth = m_input.GetSize().x;
            int displayHeight = m_input.GetSize().y;

            if (displayWidth > maxWidth)
            {
                float ratio = displayWidth / maxWidth;
                displayWidth /= ratio;
                displayHeight /= ratio;
            }

            if (displayHeight > maxHeight)
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