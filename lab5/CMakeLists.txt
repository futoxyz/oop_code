cmake_minimum_required(VERSION 3.20)
project(lab5 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(FetchContent)
set(BUILD_GMOCK OFF)
set(gtest_force_shared_crt ON)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

option(ENABLE_ASAN "Build with AddressSanitizer (GCC or Clang)" OFF)
if(ENABLE_ASAN)
    if(MSVC OR NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
        message(FATAL_ERROR "ENABLE_ASAN requires GCC or Clang with AddressSanitizer support")
    endif()
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

set(sources book.cpp book_tree.cpp)
# add_executable(main main.cpp ${sources})
add_executable(tests tests.cpp ${sources})
target_link_libraries(tests PRIVATE GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests DISCOVERY_MODE PRE_TEST)
