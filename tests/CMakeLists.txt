find_package(GTest)
include(GoogleTest)


add_executable(StaticVectorTests tests.cpp)
target_compile_options(StaticVectorTests 
    PUBLIC -fsanitize=address -fprofile-arcs -ftest-coverage)
target_link_options(StaticVectorTests 
    PUBLIC -fsanitize=address -ftest-coverage)
target_link_libraries(StaticVectorTests svector GTest::gtest_main gcov)
set_target_properties(StaticVectorTests
    PROPERTIES
    CXX_STANDARD 20
)

gtest_discover_tests(StaticVectorTests)
