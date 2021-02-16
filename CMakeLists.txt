project(gpgpu_sample)
cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

add_subdirectory(contrib)
add_subdirectory(src)