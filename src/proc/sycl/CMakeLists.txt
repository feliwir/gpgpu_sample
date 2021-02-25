find_package(ComputeCpp REQUIRED)

if(ComputeCpp_FOUND)
    add_library(libprocsycl STATIC)
    message(STATUS "Found SYCL (ComputeCPP)")
    include_directories(${ComputeCpp_INCLUDE_DIRS})

    set(SYCL_SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLImage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLToneMappingProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLToneMappingProcessor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLBrightnessProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLBrightnessProcessor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLSaturationProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SYCLSaturationProcessor.hpp)

    target_sources(libprocsycl PRIVATE ${SYCL_SRC_FILES})
    target_include_directories(libprocsycl PUBLIC ${CMAKE_SOURCE_DIR}/contrib/glm)

    add_sycl_to_target(TARGET libprocsycl SOURCES ${SYCL_SRC_FILES})
    target_link_libraries(libproc PUBLIC libprocsycl)
endif()
