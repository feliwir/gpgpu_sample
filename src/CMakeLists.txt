set(IMGUI_CONTRIB   ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui_draw.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui_tables.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui_widgets.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_opengl3.cpp 
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl.cpp)

set(SRC_FILES main.cpp Window.cpp Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/CPUBrightnessProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu/CPUSaturationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mtcpu/MTCPUBrightnessProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mtcpu/MTCPUSaturationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLBrightnessProcessor.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.cpp
    ${CMAKE_SOURCE_DIR}/contrib/tinyfiledialogs/tinyfiledialogs.c
    ${IMGUI_CONTRIB})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenMP)

add_executable(gpgpu_sample ${SRC_FILES})
target_link_libraries(gpgpu_sample SDL2 SDL2main OpenGL::GL GLEW::glew stdc++fs OpenMP::OpenMP_CXX)

find_package(ComputeCpp REQUIRED)

if(ComputeCpp_FOUND)
    message(STATUS "Found SYCL (ComputeCPP)")
    include_directories(${ComputeCpp_INCLUDE_DIRS})

    set(SRC_SYCL_FILES
        # ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.cpp
        # ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.hpp
        # ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.cpp
        # ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationKernel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sycl/SYCLSaturationProcessor.hpp)

    target_sources(gpgpu_sample PRIVATE ${SRC_SYCL_FILES})

    add_sycl_to_target(TARGET "gpgpu_sample" ${SRC_SYCL_FILES})
endif()


target_include_directories(gpgpu_sample PUBLIC 
    ${CMAKE_SOURCE_DIR}/contrib/glm
    ${CMAKE_SOURCE_DIR}/contrib/stb
    ${CMAKE_SOURCE_DIR}/contrib/tinyfiledialogs
    ${CMAKE_SOURCE_DIR}/contrib/imgui
    ${CMAKE_SOURCE_DIR}/contrib/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Freetype)

if(FREETYPE_FOUND)
    message(STATUS "Using freetype for FontRendering")
    target_compile_definitions(gpgpu_sample PRIVATE HAS_FREETYPE_SUPPORT)
    target_sources(gpgpu_sample PRIVATE ${CMAKE_SOURCE_DIR}/contrib/imgui/misc/freetype/imgui_freetype.cpp)
    target_include_directories(gpgpu_sample PRIVATE ${CMAKE_SOURCE_DIR}/contrib/imgui/misc/freetype/)
    target_link_libraries(gpgpu_sample Freetype::Freetype)
endif()

