project(gpgpu_sample)
cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

add_subdirectory(contrib)
add_subdirectory(src)