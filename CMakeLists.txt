cmake_minimum_required(VERSION 3.10)
project(Task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(googletest)

add_executable(
    Task
    main.cpp
)

target_link_libraries(
    Task
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Task)
