cmake_minimum_required(VERSION 3.24)

project(
    NAME StaticVector
    VERSION 0.1.0
    DESCRIPTION "Vector like container with static memroy allocation"
)

add_library(svector INTERFACE)
target_include_directories(svector INTERFACE include/)
set_target_properties(svector
    PROPERTIES
    CXX_STANDARD 23
)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
