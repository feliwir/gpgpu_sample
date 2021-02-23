set(IMGUI_CONTRIB   ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui_draw.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui_tables.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/imgui_widgets.cpp
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_opengl3.cpp 
                    ${CMAKE_SOURCE_DIR}/contrib/imgui/backends/imgui_impl_sdl.cpp)

set(SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp 
    ${CMAKE_SOURCE_DIR}/contrib/tinyfiledialogs/tinyfiledialogs.c
    ${IMGUI_CONTRIB})

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(app ${SRC_FILES})
target_link_libraries(app PRIVATE SDL2 SDL2main OpenGL::GL GLEW::glew stdc++fs libproc)

target_include_directories(app PUBLIC 
    ${CMAKE_SOURCE_DIR}/contrib/glm
    ${CMAKE_SOURCE_DIR}/contrib/tinyfiledialogs
    ${CMAKE_SOURCE_DIR}/contrib/imgui
    ${CMAKE_SOURCE_DIR}/contrib/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/../)

find_package(Freetype)

if(FREETYPE_FOUND)
    message(STATUS "Using freetype for FontRendering")
    target_compile_definitions(app PRIVATE HAS_FREETYPE_SUPPORT)
    target_sources(app PRIVATE ${CMAKE_SOURCE_DIR}/contrib/imgui/misc/freetype/imgui_freetype.cpp)
    target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/contrib/imgui/misc/freetype/)
    target_link_libraries(app PRIVATE Freetype::Freetype)
endif()