add_library(libproc STATIC)
add_subdirectory(cpu)
add_subdirectory(sycl)

target_link_libraries(libproc PUBLIC libimageio)