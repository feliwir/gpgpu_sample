add_library(libproc STATIC ${CMAKE_CURRENT_SOURCE_DIR}/IImage.cpp)
target_include_directories(libproc PRIVATE ${CMAKE_SOURCE_DIR}/contrib/stb)
add_subdirectory(cpu)
add_subdirectory(sycl)