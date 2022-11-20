cmake_minimum_required(VERSION 3.24)

project(
    StaticVector
    VERSION 0.1.0
    DESCRIPTION "Vector like container with static memroy allocation"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(STATICVEC_SUPPORT_IOSTREAM "Support <iostream> for StaticVector" ON)

add_library(svector INTERFACE "include/staticvector.hpp")
target_include_directories(svector INTERFACE include/)
set_target_properties(svector
    PROPERTIES
    CXX_STANDARD 23
)

if (STATICVEC_SUPPORT_IOSTREAM)
    target_compile_definitions(svector INTERFACE -DSTATICVEC_SUPPORT_IOSTREAM=1)
endif()

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
