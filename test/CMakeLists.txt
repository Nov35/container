cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)

add_executable(ListTests list_tests.cpp)

target_link_libraries(ListTests PUBLIC
    gtest_main
    Container
)

add_test(
    NAME ListTests
    COMMAND ListTests
)
