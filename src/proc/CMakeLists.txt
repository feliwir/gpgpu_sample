add_library(libproc STATIC)
target_include_directories(libproc PUBLIC ${CMAKE_SOURCE_DIR}/contrib/glm)

add_subdirectory(cpu)
add_subdirectory(sycl)

target_link_libraries(libproc PUBLIC libimageio)