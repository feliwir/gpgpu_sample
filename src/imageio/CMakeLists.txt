
add_library(libimageio STATIC ${CMAKE_CURRENT_SOURCE_DIR}/ImageData.cpp)
target_include_directories(libimageio PRIVATE ${CMAKE_SOURCE_DIR}/contrib/stb)
