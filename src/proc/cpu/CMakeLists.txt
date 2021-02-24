set(CPU_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUImage.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUBrightnessProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUSaturationProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPUToneMappingProcessor.cpp)

target_sources(libproc PRIVATE ${CPU_SRC_FILES})

find_package(OpenMP)
target_link_libraries(libproc PRIVATE OpenMP::OpenMP_CXX)