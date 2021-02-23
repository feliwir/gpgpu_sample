add_library(libproccpu STATIC)

set(CPU_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUImage.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUBrightnessProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUSaturationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUToneMappingProcessor.cpp)

target_sources(libproccpu PRIVATE ${CPU_SRC_FILES})

find_package(OpenMP)
target_link_libraries(libproccpu PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(libproc PRIVATE libproccpu)