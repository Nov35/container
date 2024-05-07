cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)

project(Container VERSION 1.0.0 LANGUAGES C CXX)

include(FetchContent)
include(CTest)

FetchContent_Declare(
    googletest-distribution
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(googletest-distribution)

add_subdirectory(src)
add_subdirectory(test)
