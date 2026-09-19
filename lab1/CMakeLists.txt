cmake_minimum_required(VERSION 4.0.0)
project(lab1 CXX)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)
add_executable(main main.cpp matrix_ops.cpp)
add_executable(tests tests.cpp matrix_ops.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

# cd lab1
# cmake -S . -B build
# cmake --build build
# ./build/???
